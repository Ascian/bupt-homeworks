#include <semaphore.h>

#include <iostream>

#include "logging.h"
#include "contiguous_memory.h"


//控制线程数量的信号量
sem_t kQueueNum;
//output输出流
Log kLog("output.txt");


int main(int argc, char* argv[])
{
    if (argc < 7)
        return 0;

    int memory_size = stoi(argv[1]);  //内存大小
    int process_max = stoi(argv[2]);  //设置进程数量
    int time_max = stoi(argv[3]);  //进程运行最大花费时间
    int memory_max = stoi(argv[4]);  //进程运行最大耗费内存
    ContiguousMemory::Method method;
    string temp = string(argv[5]);
    if (temp == "-ff")
        method = ContiguousMemory::Method::FIRST_FIT;
    else if (temp == "-bf")
        method = ContiguousMemory::Method::BEST_FIT;
    else if (temp == "-wf")
        method = ContiguousMemory::Method::WORST_FIT;
    else 
        return 0;
    bool is_preemptive;
    temp = string(argv[6]);
    if (temp == "-p")
        is_preemptive = true;
    else if (temp == "-np")
        is_preemptive = false;
    else
        return 0;

    ContiguousMemory* memory = new ContiguousMemory(memory_size, process_max, 
        is_preemptive, method, time_max / 2);

    Process* processes = new Process[process_max];
    for (int i = 0; i < process_max; i++)
        processes[i].is_used = false;

    sem_init(&kQueueNum, 0, process_max);

    srand((unsigned)time(NULL));

    int total_num = 0;
    while (true)
    {
        sem_wait(&kQueueNum);
        //寻找空闲进程
        int i = 0;
        for (; i < process_max; i++)
        {
            if (!processes[i].is_used)
                break;
        }
        processes[i].is_used = true;
        processes[i].id = total_num++;
        //生成范围[1, time_max]的随机数作为花费时间
        processes[i].spend_time = rand() % time_max + 1;
        //生成范围[1, memory_max]的随机数作为花费内存
        processes[i].memory_size = rand() % memory_max + 1;

        kLog << "Create " << processes[i].id << " " << processes[i].memory_size <<
            " " << processes[i].spend_time << "\n";

        memory->Execute(&processes[i]);
    }

    delete memory;
    delete[] processes;
}
