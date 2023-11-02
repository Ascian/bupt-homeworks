#pragma comment(lib, "pthreadVC2.lib")
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <string>
#include <random>

#include "logging.h"
#include "order.h"
#include "readers_first.h"
#include "writers_first.h"

//控制线程数量的信号量
sem_t kQueueNum;
//output输出流
Log kOutputLog("output.txt");


int main(int argc, char* argv[])
{
    if (argc < 4)
        return 0;

    int data_num = stoi(argv[2]);  //设置线程最大数量
    int time_max = stoi(argv[3]);  //读或写最大花费时间
    sem_init(&kQueueNum, 0, data_num);  
    pthread_t* threads = new pthread_t[data_num];
    Order* orders = new Order[data_num];
    for (int i = 0; i < data_num; i++)
        orders[i].is_used = false;
    srand((unsigned)time(NULL));

    int total_num = 0;
    if (string(argv[1]) == "rf")
    {
        //读者优先
        kOutputLog << "readers first\n";
        readers_first::init();
        while (true)
        {
            sem_wait(&kQueueNum);
            //寻找空闲线程
            int i = 0;
            for (; i < data_num; i++)
            {
                if (!orders[i].is_used)
                    break;
            }
            orders[i].is_used = true;
            orders[i].id = total_num++;
            //生成范围[1, time_max]的随机数作为花费时间
            orders[i].spend_time = rand() % time_max + 1;
            //随机生成读者或写者
            if (rand() % 2 == 1)
            {
                kOutputLog << "Create W" << orders[i].id << orders[i].spend_time << "\n";
                pthread_create(&threads[i], nullptr, readers_first::writer, &orders[i]);
            }
            else
            {
                kOutputLog << "Create R" << orders[i].id << orders[i].spend_time << "\n";
                pthread_create(&threads[i], nullptr, readers_first::reader, &orders[i]);
            }
        }
        readers_first::destroy();
    }
    else if (string(argv[1]) == "wf")
    {
        //写者优先
        kOutputLog << "writers first\n";
        writers_first::init();
        while (true)
        {
            sem_wait(&kQueueNum);
            //寻找空闲线程
            int i = 0;
            for (; i < data_num; i++)
            {
                if (!orders[i].is_used)
                    break;
            }
            orders[i].is_used = true;
            orders[i].id = total_num++;
            //生成范围[1, time_max]的随机数作为花费时间
            orders[i].spend_time = rand() % time_max + 1;
            //随机生成读者或写者
            if (rand() % 2 == 1)
            {
                kOutputLog << "Create W" << orders[i].id << orders[i].spend_time << "\n";
                pthread_create(&threads[i], nullptr, writers_first::writer, &orders[i]);
            }
            else
            {
                kOutputLog << "Create R" << orders[i].id << orders[i].spend_time << "\n";
                pthread_create(&threads[i], nullptr, writers_first::reader, &orders[i]);
            }
        }
        writers_first::destroy();
    }
    delete[] threads;
    delete[] orders;
}
