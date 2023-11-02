#include <stdio.h>
#include <string.h>

#include "protocol.h"
#include "datalink.h"

#define DATA_TIMER 1200            //�ط���ʱ
#define ACK_TIMER 300              //ACK��ʱ
#define MAX_SEQ 21                  //������
#define NR_BUFS ((MAX_SEQ + 1) / 2) //���ڴ�С

struct FRAME {
	unsigned char kind;          //֡����
	unsigned char ack;           //ACK���
	unsigned char seq;           //֡���
	unsigned char data[PKT_LEN]; //����
	unsigned int padding;        //CRCԤ��λ
};

//���ͷ�����
static unsigned char ack_expected = 0;       //���ʹ����½�
static unsigned char next_frame_to_send = 0; //���ʹ����Ͻ�
static unsigned char nbuffered = 0;          //�ѻ����֡��
static unsigned char out_buf[NR_BUFS][PKT_LEN]; //���ͻ�����������δ�յ���ӦACK��֡

//���շ�����
static unsigned char frame_expected = 0; //���մ����½�
static unsigned char too_far = NR_BUFS;  //���մ����Ͻ�
static unsigned char in_buf[NR_BUFS][PKT_LEN]; //���ջ��������������յ���֡
static unsigned char arrived[NR_BUFS];         //���ջ�����ʹ�����

static int phl_ready = 0; //�����׼���ź�

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
	if (seq != next_frame_to_send) //�����ط�֡��֮�󻺴�֡�ļ�ʱ��
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
	int event;      //�¼�����
	int arg;        //timer���
	struct FRAME f; //�������յ�֡
	int len = 0;    //�����������ݵĳ���
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
					//crcУ��������̷���NAK֡
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