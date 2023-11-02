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
