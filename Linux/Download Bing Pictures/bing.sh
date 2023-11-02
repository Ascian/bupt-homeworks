#! /bin/env bash

# Default paths
bing="bing"
temp="bing/.temp"
date="bing/.temp/.date"
page="bing/.temp/.page"

# Default arguments
start_page=1
max_retry_times=10
max_nproc=5
wait_interval=5

# Parse the arguments
for arg; do
    if [[ "$arg" =~ retry=[0-9]+ ]]; then
        max_retry_times=$(sed 's/retry=\(.*\)/\1/' <<<$arg)
    elif [[ "$arg" =~ nproc=[1-9][0-9]* ]]; then
        max_nproc=$(sed 's/nproc=\(.*\)/\1/' <<<$arg)
    elif [[ "$arg" =~ wait=[0-9]+ ]]; then
        wait_interval=$(sed 's/wait=\(.*\)/\1/' <<<$arg)
    else
        echo -e "\033[31mInvalid argument $arg\033[0m"
        exit 1
    fi
done

# Get the max number of page
echo -e "\033[33mTrying to get the max number of page...\033[0m"
retry_times=0
end_page="$(wget https://bing.ioliu.cn -qO- | egrep -o '<span>[0-9]+ / [0-9]+</span>' | sed 's/<span>\([0-9]\+\) \/ \([0-9]\+\)<\/span>/\2/')"
# Continue trying to get the max number of page until the download is successful
# or the number of retries exceeds the maximum number of retries
while [ $? -ne 0 ] && [ $retry_times -lt $max_retry_times ]; do
    retry_times=$(expr $retry_times + 1)
    echo -e "\033[31mFailed to get the max number of page, retrying $retry_times...\033[0m"
    sleep $wait_interval
    end_page="$(wget https://bing.ioliu.cn -qO- | egrep -o '<span>[0-9]+ / [0-9]+</span>' | sed 's/<span>\([0-9]\+\) \/ \([0-9]\+\)<\/span>/\2/')"
done
# If the number of retries exceeds the maximum number of retries, exit
if [ $retry_times -ge $max_retry_times ]; then
    echo -e "\033[31mFailed to get the max number of page after $max_retry_times retries\033[0m"
    exit 1
else
    echo -e "\033[32mSucceed to get the max number of page: $end_page\033[0m"
fi

if [ ! -d $bing ]; then
    mkdir $bing
    if [ $? -ne 0 ]; then
        echo -e "\033[31mFailed to create directory bing\033[0m"
        exit 1
    fi
fi

# If since the last download, the date has not changed, set start page as the last page of the last download
# Otherwise, set start page as 1
if [ -d $temp ]; then
    last_date=$(cat $date)
    if [[ "$last_date" == "$(date +%Y-%m-%d)" ]]; then
        start_page=$(expr "$(cat $page)" + 1)
    fi
else
    mkdir $temp
    if [ $? -ne 0 ]; then
        echo -e "\033[31mFailed to create directory $temp\033[0m"
        exit 1
    fi
    echo 0 >$page
fi
date +%Y-%m-%d >$date

# Download the background image of page $1
download_bg_from_page() {
    # Default paths
    bing="bing"
    temp="bing/.temp"
    date="bing/.temp/.date"
    page="bing/.temp/.page"

    local html="$temp/.$1.html"
    echo -e "\033[33mDownloading the html file of page $1...\033[0m"

    local retry_times=0
    wget https://bing.ioliu.cn/?p=$1 -qO $html
    # Continue trying to download the html file until the download is successful
    # or the number of retries exceeds the maximum number of retries
    while [ $? -ne 0 ] && [ $retry_times -lt $max_retry_times ]; do
        retry_times=$(expr $retry_times + 1)
        echo -e "\033[31mFailed to download the html file of page $1, retrying $retry_times...\033[0m"
        sleep $wait_interval
        wget https://bing.ioliu.cn/?p=$1 -qO $html
    done

    # If the number of retries exceeds the maximum number of retries, delete the html file
    if [ $retry_times -ge $max_retry_times ]; then
        echo -e "\033[31mFailed to download the html file of page $1 after $max_retry_times retries\033[0m"
        rm "$temp/.$1.html" >/dev/null 2>&1
    # If the download is successful, print a success message and download the background image of page $1
    else
        echo -e "\033[32mSucceed to download the html file of page $1\033[0m"

        # Create a directory to store the temporary files of page $1
        local temp_dict="$temp/$1"
        if [ ! -d $temp_dict ]; then
            mkdir $temp_dict
        fi
        # If this page is the max page, set the max page as the last page of the last download
        if [ $1 -gt "$(cat $page)" ]; then
            echo $1 >$page
        fi

        # Download the background image of page $1
        sed 's/<div class="item">/\n<div class="item">/g' "$html" | egrep '<div class="item">.*</div></div></div>' | while read line; do
            # Get the date, description and url of the background image
            local img_date="$(egrep -o '<p class="calendar"><i class="icon icon-calendar"><\/i><em class="t">[0-9]+-[0-9]+-[0-9]+<\/em><\/p>' <<<"$line" | sed 's/<p class="calendar"><i class="icon icon-calendar"><\/i><em class="t">\([0-9]\+\)-\([0-9]\+\)-\([0-9\]\+\)<\/em><\/p>/\1-\2-\3/')" >/dev/null 2>&1
            local img_descript="$(egrep -o '<div class="description"><h3>[^ ]*(, [^ ]*)*' <<<"$line" | sed 's/<div class="description"><h3>\(.*\)/\1/')" >/dev/null 2>&1
            local img_url="$(egrep -o '<a class="ctrl download" photo="[^"]*" href="[^"]*" target="_blank" rel="nofollow" title="[^"]*"><i class="icon icon-download"></i><em class="t">UHD</em>' <<<$line | sed 's/<a class="ctrl download" photo="[^"]*" href="\([^"]*\)" target="_blank" rel="nofollow" title="[^"]*"><i class="icon icon-download"><\/i><em class="t">UHD<\/em>/\1/')" >/dev/null 2>&1

            local file_name="$img_date $img_descript.jpg"
            local file_path="$bing/$file_name"
            # check if the background image has been downloaded
            if [ ! -f "$file_path" ]; then
                echo -e "\033[33mDownloading $img_date $img_descript.jpg...\033[0m"
                local temp_file_path="$temp_dict/$file_name"

                local retry_times=0
                wget "$img_url" -qO "$temp_file_path"
                # Continue trying to download the background image until the download is successful
                # or the number of retries exceeds the maximum number of retries
                while [ $? -ne 0 ] && [ $retry_times -lt $max_retry_times ]; do
                    retry_times=$(expr $retry_times + 1)
                    echo -e "\033[31mFailed to download $file_name, retrying $retry_times...\033[0m"
                    sleep $wait_interval
                    wget "$img_url" -qO "$temp_file_path"
                done

                # If the number of retries exceeds the maximum number of retries, delete the temporary file
                if [ $retry_times -ge $max_retry_times ]; then
                    echo -e "\033[31mFailed to download $file_name after $max_retry_times retries\033[0m"
                    rm "$temp_file_path"
                # If the download is successful, rename the temporary file to the target file
                else
                    mv "$temp_file_path" "$file_path"
                    echo -e "\033[32mSucceed to download $file_name\033[0m"
                fi
            fi
        done
        rm -rf "$temp_dict"
        rm "$html"
    fi

}

# Download the background images of the pages from $start_page to $end_page
# and the page not downloaded completely last time
for page_num in $(ls $temp) $(seq $start_page $end_page); do
    # Wait until the number of background processes is less than the maximum number of background processes
    while [ "$(jobs -r | wc -l)" -ge $max_nproc ]; do
        sleep 1
    done
    download_bg_from_page $page_num &
done
