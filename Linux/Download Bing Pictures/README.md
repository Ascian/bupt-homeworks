# 下载 bing 图库中的图片

## 名称

bing.sh - 下载 bing 所有每日高清壁纸

## 用法

    ./bing.sh [options]

## 描述

将[必应每日高清壁纸](https://bing.ioliu.cn)中照片全部下载下来存放到本地 bing 目录。下载后的文件命名为 “日期 说明.jpg” 例如：2019-08-03 野花草甸上的一只欧亚雕鸮，德国莱茵兰-普法尔茨.jpg

* 支持断点重启和不重复下载已下载的图片
* 支持并发下载

## 选项

* nproc=number
    设置并发下载进程数。默认为 5 
* retry=number
    设置下载失败重试次数。默认为 10
* wait=number
    设置下载失败后的重试等待时间。默认为 5 秒

## 演示

以 nproc=5 retry=5 wait=10 为例：
![bing](img/bing.png)

断点重启：
![bing](img/bing2.png)

下载结果：
![bing](img/bing3.png)

## 代码解释

* 获取最大页面数

    ``` bash

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
    ```
    * 首先通过 `wget https://bing.ioliu.cn -qO-` 下载必应每日高清壁纸首页
    * 通过观察 html 文件，可以发现最大页面数在 `<span>[0-9]+ / [0-9]+</span>` 中，例如 `<span>1 / 100</span>`，所以通过 `egrep -o '<span>[0-9]+ / [0-9]+</span>'` 获取到 `<span>1 / 100</span>`，然后通过 `sed 's/<span>\([0-9]\+\) \/ \([0-9]\+\)<\/span>/\2/'` 获取到最大页面 `100` 
    * 如果下载失败，就会重试，直到下载成功或者重试次数超过最大重试次数
    * 如果重试次数超过最大重试次数，就会退出程序
* 获取断点信息
    ``` bash
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
    ```
    * `$date` 文件为 bing/.temp/.date，用于存放上次下载的日期。若自上次下载后，日期已经过去超过一天，则将 `$start_page` 设置为 1，表示从头开始下载，否则设置为上次下载的页数加 1。`$page` 文件为 bing/.temp/.page，用于存放上次下载的页数
* 从页面内下载图片
    ``` bash
    # Download the background image of page $1
    download_bg_from_page() 
    ```
    * 定义函数 download_bg_from_page，用于从页面内下载图片，参数为页面数
    * 首先获取页面的 html 文件
        ``` bash
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
        ```
        * 通过 `wget https://bing.ioliu.cn/?p=$1 -qO $html` 下载页面的 html 文件 到文件 `$html`，`$html` 文件为 bing/.temp/.$1.html
        * 如果下载失败，就会重试，直到下载成功或者重试次数超过最大重试次数
        * 如果重试次数超过最大重试次数，就会删除 `$html` 文件
        * 如果下载成功，首先创建临时文件夹 `$temp_dict`，文件夹用页面命名，用于存放页面内的正在下载的图片
        * 若当前页面是目前已下载的最大页面，则将 `$page` 文件内容设置为当前页面
        * 随后开始下载页面内的图片
            ``` bash
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
            ```
            * 观察发现 html 文件中关于每一张图片的信息都在 `<div class="item">` 标签中，所以可以使用 `sed 's/<div class="item">/\n<div class="item">/g' "$html"`在 `<div class="item">` 标签前添加换行，就可以将每一张图片的信息分开
            * 然后使用 `egrep '<div class="item">.*</div></div></div>'` 将其余的信息过滤掉，只保留所有图片 `<div class="item">` 标签中的信息
            * 然后使用 `while read line; do` 将每一张图片的信息读入变量 line
            * 观察每一条图片信息发现，图片的日期在 `<p class="calendar"><i class="icon icon-calendar"></i><em class="t">2023-05-05</em></p>` 标签中，所以可以使用 `egrep -o '<p class="calendar"><i class="icon icon-calendar"><\/i><em class="t">[0-9]+-[0-9]+-[0-9]+<\/em><\/p>' <<<"$line" | sed 's/<p class="calendar"><i class="icon icon-calendar"><\/i><em class="t">\([0-9]\+\)-\([0-9]\+\)-\([0-9\]\+\)<\/em><\/p>/\1-\2-\3/'` 将日期提取出来
            * 图片的描述在 `<div class="description"><h3>普埃布拉天主堂和波波卡特佩特火山，普埃布拉，墨西哥 (© Radius Images/Shutterstock)</h3>` 标签中，所以可以使用 `egrep -o '<div class="description"><h3>[^ ]*(, [^ ]*)*' <<<"$line" | sed 's/<div class="description"><h3>\(.*\)/\1/'` 将描述提取出来
            * 图片的 UHD 下载 url 在 `<a class="ctrl download" photo="5346" href="https://bing.com/th?id=OHR.Popocatepetl_ZH-CN5483138337_UHD.jpg&amp;qlt=100" target="_blank" rel="nofollow" title="Popocatepetl_ZH-CN5483138337_UHD.jpg"><i class="icon icon-download"></i><em class="t">UHD</em>` 标签中，所以可以使用 `egrep -o '<a class="ctrl download" photo="[^"]*" href="[^"]*" target="_blank" rel="nofollow" title="[^"]*"><i class="icon icon-download"></i><em class="t">UHD</em>' <<<$line | sed 's/<a class="ctrl download" photo="[^"]*" href="\([^"]*\)" target="_blank" rel="nofollow" title="[^"]*"><i class="icon icon-download"><\/i><em class="t">UHD<\/em>/\1/'` 将 url 提取出来
            * 然后将日期、描述、url 拼接成文件名，检查文件是否存在，如果不存在则使用 `wget "$img_url" -qO "$temp_file_path"` 将图片下载到临时文件夹中
            * 如果下载失败，就会重试，直到下载成功或者重试次数超过最大重试次数
            * 如果重试次数超过最大重试次数，就会删除临时文件
            * 如果下载成功，就会将临时文件重命名为目标文件，放入 bing 文件夹中
        * 当页面内所有图片都下载完成后，删除临时文件夹和 html 文件
            ``` bash
            rm -rf "$temp_dict"
            rm "$html"
            ```
* 多进程下载图片
    ```bash
    # Download the background images of the pages from $start_page to $end_page
    # and the page not downloaded completely last time
    for page_num in $(ls $temp) $(seq $start_page $end_page); do
        # Wait until the number of background processes is less than the maximum number of background processes
        while [ "$(jobs -r | wc -l)" -ge $max_nproc ]; do
            sleep 1
        done
        download_bg_from_page $page_num &
    done          
    ```
    * 若 `$temp` 文件夹内存在临时文件夹未删除，表示上次下载时该页面未下载完成，所以需要优先继续下载该页面
    * 检查当前进程数是否超过最大进程数，循环等待，直到当前进程数小于最大进程数
    * 创建一个子进程，调用 download_bg_from_page 函数下载页面中的图片

## 代码
```bash
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

```
