#include <pthread.h>
#include <semaphore.h>
#include <Windows.h>

#include "logging.h"
#include "memory_management_system.h"

//控制进程数量的信号量
sem_t kQueueNum;
//控制对内存系统的访问
pthread_mutex_t kMutex;
//output输出流
Log kLog("output.txt");

struct Process 
{
    MemoryManagementSystem* memory;
    bool is_used;
    int id;
    int command_count;
    int page_count;
};

void* Execute(void* param);

int main(int argc, char* argv[])
{
    if (argc < 6)
        return 0;

    int main_memory_size = stoi(argv[1]);  //主存大小
    int virtual_memory_size = stoi(argv[2]);  //虚存大小
    int process_count = stoi(argv[3]);  //设置进程数量
    int command_max = stoi(argv[4]);  //最大进程访问内存指令数
    int page_max = stoi(argv[5]);  //进程运行最大耗费内存页数
    
    MemoryManagementSystem* memory = new MemoryManagementSystem(main_memory_size, virtual_memory_size);

    pthread_t* threads = new pthread_t[process_count];
    Process* processes = new Process[process_count];
    for (int i = 0; i < process_count; i++)
    {
        processes[i].memory = memory;
        processes[i].is_used = false;
    }

    sem_init(&kQueueNum, 0, process_count);
    pthread_mutex_init(&kMutex, nullptr);

    srand((unsigned)time(NULL));

    int total_num = 0;
    while (true)
    {
        sem_wait(&kQueueNum);
        //寻找空闲进程
        int i = 0;
        for (; i < process_count; i++)
        {
            if (!processes[i].is_used)
                break;
        }
        processes[i].is_used = true;
        processes[i].id = total_num++;
        //生成范围[1, command_max]的随机数作为访问内存的次数
        processes[i].command_count = rand() % command_max + 1;
        //生成范围[1, page_max]的随机数作为花费内存
        processes[i].page_count = rand() % page_max + 1;

        kLog << "Create " << processes[i].id << " " << processes[i].command_count <<
            " " << processes[i].page_count << "\n";

        pthread_create(&threads[i], nullptr, Execute, &processes[i]);
    }

    sem_destroy(&kQueueNum);
    pthread_mutex_destroy(&kMutex);
    delete memory;
    delete[] processes;
}

void* Execute(void* param)
{
    Process* process = (Process*)param;

    //添加程序到内存中
    pthread_mutex_lock(&kMutex);
    bool is_added = process->memory->AddProcess(process->id, process->page_count);
    pthread_mutex_unlock(&kMutex);
    if (!is_added)
        kLog << "Virtual-memory-full " << process->id << "\n";
    else
    {
        int page_num = 0;
        for (int i = 0; i < process->command_count && page_num != MemoryManagementSystem::INVALID; i++)
        {
            Sleep(1000);

            //随机生成虚拟页码，有 1 / (page_count + 1)的概率页码非法
            int virtual_page_num = rand() % (process->page_count + 1);
            pthread_mutex_lock(&kMutex);
            kLog << "Access " << process->id << " " << virtual_page_num << "\n";
            page_num = process->memory->Access(process->id, virtual_page_num);
            pthread_mutex_unlock(&kMutex);
        }
         
        //删除内存中的进程
        pthread_mutex_lock(&kMutex);
        if (page_num != MemoryManagementSystem::INVALID)
            kLog << "Normal-exit " << process->id << "\n";
        else
            kLog << "Abnormal-exit " << process->id << "\n";
        process->memory->DeleteProcess(process->id);
        pthread_mutex_unlock(&kMutex);
    }

    process->is_used = false;
    sem_post(&kQueueNum);
    return nullptr;
}

