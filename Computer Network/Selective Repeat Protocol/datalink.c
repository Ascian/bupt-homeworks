#include <stdio.h>
#include <string.h>

#include "protocol.h"
#include "datalink.h"

#define DATA_TIMER 1200            //重发计时
#define ACK_TIMER 300              //ACK计时
#define MAX_SEQ 21                  //最大序号
#define NR_BUFS ((MAX_SEQ + 1) / 2) //窗口大小

struct FRAME {
	unsigned char kind;          //帧种类
	unsigned char ack;           //ACK序号
	unsigned char seq;           //帧序号
	unsigned char data[PKT_LEN]; //数据
	unsigned int padding;        //CRC预留位
};

//发送方变量
static unsigned char ack_expected = 0;       //发送窗口下界
static unsigned char next_frame_to_send = 0; //发送窗口上界
static unsigned char nbuffered = 0;          //已缓存的帧数
static unsigned char out_buf[NR_BUFS][PKT_LEN]; //发送缓冲区，缓存未收到对应ACK的帧

//接收方变量
static unsigned char frame_expected = 0; //接收窗口下界
static unsigned char too_far = NR_BUFS;  //接收窗口上界
static unsigned char in_buf[NR_BUFS][PKT_LEN]; //接收缓冲区，缓存已收到的帧
static unsigned char arrived[NR_BUFS];         //接收缓冲区使用情况

static int phl_ready = 0; //物理层准备信号

static void inc(unsigned char* seq) {
	*seq = ((*seq) + 1) % (MAX_SEQ + 1);
}

static int between(unsigned char seq, unsigned char begin, unsigned char end) {
	return ((begin <= seq) && (seq < end)) ||
	       ((end < begin) && (begin <= seq)) ||
	       ((seq < end) && (end < begin));
}

static void put_frame(unsigned char *frame, int len)
{
	*(unsigned int *)(frame + len) = crc32(frame, len);
	send_frame(frame, len + 4);
	phl_ready = 0;
}

static void send_data_frame(unsigned char seq) {
	struct FRAME s;
	s.kind = FRAME_DATA;
	s.seq = seq;
	s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);
	memcpy(s.data, out_buf[seq % NR_BUFS], PKT_LEN);

	dbg_frame("Send DATA %d %d, ID %d\n", s.seq, s.ack, *(short *)s.data);

	put_frame((unsigned char *)&s, 3 + PKT_LEN);
	if (seq != next_frame_to_send) //重置重发帧和之后缓存帧的计时器
		for (; seq != next_frame_to_send; inc(&seq))
			start_timer(seq % NR_BUFS, DATA_TIMER);
	else
		start_timer(seq % NR_BUFS, DATA_TIMER);
	stop_ack_timer();
}

static void send_ack_frame(void)
{
	struct FRAME s;

	s.kind = FRAME_ACK;
	s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

	dbg_frame("Send ACK  %d\n", s.ack);

	put_frame((unsigned char *)&s, 2);
	stop_ack_timer();
}

static void send_nak_frame(unsigned char seq)
{
	struct FRAME s;

	s.kind = FRAME_NAK;
	s.ack = seq;

	dbg_frame("Send NAK  %d\n", s.ack);

	put_frame((unsigned char *)&s, 2);
}


int main(int argc, char **argv)
{
	int event;      //事件类型
	int arg;        //timer序号
	struct FRAME f; //物理层接收的帧
	int len = 0;    //物理层接收数据的长度
	int i;
	for (i = 0; i < NR_BUFS; i++) {
		arrived[i] = 0;
	}

	protocol_init(argc, argv);
	lprintf("Designed by Chen Yu, build: " __DATE__ "  "__TIME__"\n");

	disable_network_layer();

	while (1) {
		event = wait_for_event(&arg);

		switch (event) {
		case NETWORK_LAYER_READY:
			nbuffered++;
			get_packet(out_buf[next_frame_to_send % NR_BUFS]);
			send_data_frame(next_frame_to_send);
			inc(&next_frame_to_send);
			break;

		case PHYSICAL_LAYER_READY:
			phl_ready = 1;
			break;

		case FRAME_RECEIVED:
			len = recv_frame((unsigned char *)&f, sizeof f);
			if (len < 5 || crc32((unsigned char *)&f, len) != 0) {
				dbg_event("**** Receiver Error, Bad CRC Checksum\n");
				if (len >= 5 ) {
					//crc校验出错，立刻发出NAK帧
					send_nak_frame(f.seq);
				}
				break;
			}
			if (f.kind == FRAME_ACK)
				dbg_frame("Recv ACK %d\n", f.ack);
			if (f.kind == FRAME_DATA) {
				dbg_frame("Recv DATA %d %d, ID %d\n", f.seq,
					  f.ack, *(short *)f.data);
				if (f.seq != frame_expected) {
					start_ack_timer(ACK_TIMER);
				}
				if (between(f.seq,frame_expected, too_far) &&
				    arrived[f.seq % NR_BUFS] == 0){
					arrived[f.seq % NR_BUFS] = 1;
					memcpy(in_buf[f.seq % NR_BUFS], f.data, PKT_LEN);
					while (arrived[frame_expected % NR_BUFS]) {
						put_packet(in_buf[frame_expected % NR_BUFS] ,len - 7);
						arrived[frame_expected % NR_BUFS] = 0;
						inc(&frame_expected);
						inc(&too_far);
						start_ack_timer(ACK_TIMER);
					}
				}
			}

			if (f.kind == FRAME_NAK) {
				dbg_frame("Recv NAK %d\n", f.ack);
				if (between(f.ack,ack_expected,next_frame_to_send)) {
					send_data_frame(f.ack);
				}
				break;
			}

			while(between(f.ack,ack_expected,next_frame_to_send)) {
				nbuffered--;
				stop_timer(ack_expected % NR_BUFS);
				inc(&ack_expected);
			}
			break;

		case DATA_TIMEOUT:
			dbg_event("---- DATA %d timeout\n", arg);
			send_data_frame(ack_expected);
			break;

		case ACK_TIMEOUT:
			dbg_event("---- ACK timeout\n");
			send_ack_frame();
		}

		if (nbuffered < NR_BUFS && phl_ready)
			enable_network_layer();
		else {
			if (nbuffered >= NR_BUFS)
				dbg_event("---- Buffer full\n");
			disable_network_layer();
		}	
	}
}