# 生成 TCP 活动状况报告

## 名称

tcp_report.sh - 生成 TCP 活动状况报告

## 用法

    ./tcp_report.sh 

## 描述

隔1分钟生成1行信息：当前时间；这一分钟内TCP发送了多少报文；接收了多少报文；收发报文总数；行尾给出符号+或-或空格（+表示这分钟收发报文数比上分钟多10包以上，差别在10包或以内用空格，否则用符号-）。

## 演示

![tcp](img/tcp.png)

## 代码解释

* 获取过去总收发报文数
  * 由于 `netstat --statistics` 的输出按行划分且格式清晰，因此可以用 `awk` 来处理。
  * 包含过去总收发报文数的行格式如下：
     
        2069 segments received
        2077 segments sent out
  * 使用 `awk` 处理的命令如下：
        
        total_received=$(netstat --statistics | awk '/[0-9]+ segments received/{ printf("%s",$1) }')
        total_sent=$(netstat --statistics | awk '/[0-9]+ segments sent out/{ printf("%s",$1) }')
  * 或者由 `awk` 返回命令，配合使用 `eval` 执行命令，能够实现同时获取上一分钟收发报文数和当前总收发报文数：

        eval "$(netstat --statistics | awk '/[0-9]+ segments received/{ printf("received=`expr %s - $total_received`; total_received=%s",$1,$1) }')"
        eval "$(netstat --statistics | awk '/[0-9]+ segments sent out/{ printf("sent=`expr %s - $total_sent`; total_sent=%s",$1,$1) }')"
* 获取当前时间
  * 使用 `date` 命令可以返回格式化的时间：

        time="$(date '+%Y-%m-%d %H:%M')"

## 代码

```bash

#! /bin/env bash

# Get the number of segments received and sent out so far.
total_received=$(netstat --statistics | awk '/[0-9]+ segments received/{ printf("%s",$1) }')
total_sent=$(netstat --statistics | awk '/[0-9]+ segments sent out/{ printf("%s",$1) }')

# Get the current time.
time="$(date '+%Y-%m-%d %H:%M')"

echo $time
echo "Total received segments: $total_received"
echo "Total sent segments: $total_sent"
echo ""

# Init the number of segments received and sent out last minite to 0.
pre_received_and_sent=0

while true; do
    # Output the number of segments received and sent out every minite.
    sleep 60
    time="$(date '+%Y-%m-%d %H:%M')"

    eval "$(netstat --statistics | awk '/[0-9]+ segments received/{ printf("received=`expr %s - $total_received`; total_received=%s",$1,$1) }')"
    eval "$(netstat --statistics | awk '/[0-9]+ segments sent out/{ printf("sent=`expr %s - $total_sent`; total_sent=%s",$1,$1) }')"

    cur_received_and_sent=$(expr $received + $sent)

    # If the number of segments received and sent out is greater than the previous one plus 10, then the symbol is "+",
    # otherwise, if it is less than the previous one, then the symbol is "-",
    # otherwise, the symbol is " ".
    if [ $cur_received_and_sent -gt $(expr $pre_received_and_sent + 10) ]; then
        symbol="+"
    elif [ $cur_received_and_sent -lt $pre_received_and_sent ]; then
        symbol="-"
    else
        symbol=" "
    fi

    printf "%s%6s %6s %6s %6s\n" "$time" "$received" "$sent" "$cur_received_and_sent" "$symbol"

    pre_received_and_sent=$cur_received_and_sent

done

```
