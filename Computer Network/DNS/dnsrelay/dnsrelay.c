#include <stdio.h>
#include <WinSock2.h>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h> 
#include <windows.h>
#include"Message.h"
#include"Str.h"
#include"DomainList.h"
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib, "pthreadVC2.lib")

#define VERSION 1.70//程序版本号
#define MESSAGE_MAX_LENGTH 512  //报文最大长度
#define MAX_PTHREAD 100   //线程最大数量
#define SLEEP_INTERVAL 100//周期性判断是否超时间隔
#define OVERTIME 3000 //超时应答时间
#define CACHE_CAPACITY 1000//动态cache容量
#define REFRESH_TIME 3600000000//刷新cache间隔
#define OCCUPIED 1//线程结构体被占用
#define UNOCCUPIED 0//线程结构体未被占用

//与主机DNS通信线程结构体
struct Server {
    int isOccupied;//判断是否被占用
    Str response;//接受的数据
    pthread_t tid;// 线程ID
};

//与客户端通信线程结构体
struct Client {
    short id;//clientID，用于标识不同客户
    short originId;//query原id
    int isOccupied;//判断是否被占用
    Str query;//接受的数据
    pthread_t tid;// 线程ID
    SOCKADDR_IN clientAdd;// 地址信息
};

struct Client clients[MAX_PTHREAD / 2];//与客户端通信线程
struct Server servers[MAX_PTHREAD / 2];//与主机DNS服务器通信线程
DomainList domainList = { NULL,0 };//域名-IP表
SOCKET clientSocket;//与客户端通信socket
SOCKET serverSocket;//与主机DNS服务器通信socket
SOCKADDR_IN dnsAdd;//主机DNS地址信息
SOCKADDR_IN localAdd;//本地DNS地址信息
pthread_mutex_t output;//输出线程锁
pthread_mutex_t modifyTotalQueryNum;//修改queryNum线程锁
pthread_mutex_t modifyCache;//修改域名-IP表缓存线程锁

int debugLevel = 1;//debug等级
char* dnsIp = "218.85.157.99"; //主机DNS的IP地址
char* staticDataPath = ".//dnsrelay.txt";//静态数据文件地址
int queryNum = 0;//总接受请求数量
char allowToReceive = 0;//允许接受DNS服务器数据

void initDnsrelay(int argc, char* argv[]);//处理命令行参数
void initSocket();//初始化SOCKET
void* cacheRefresh(void* arg);//刷新动态记录
void* recvFromServer(void* arg);//接受dns服务器数据
void* recvFromClient(void* arg);//接受客户端数据
void* processResponse(void* arg);//处理dns服务器数据
void* processQuery(void* arg);  //处理客户端数据
void outputRecvInfo(const char* recvIp, const int recvPort, const Str* pOriginMessage);
void outputSimpleInfo(const char* recvIp, const int isStatic, const Str* domainName, const int type);
void outputSendInfo(const char* sendIp, const int sendPort, const Str* pNewMessage, const int oldId, const int isAddAnswer);

int main(int argc, char* argv[])
{
    initDnsrelay(argc, argv);//处理命令行参数
    printf("DNSRELAY, Version %.2f, Build: " __DATE__ "  "__TIME__"\n", VERSION);
    printf("Usage: dnsrelay [-d | -dd] [<dns-server>] [<db-file>]\n\n");
    printf("Name server %s:53.\n", dnsIp);
    printf("Debug level %d.\n", debugLevel);
    printf("Bind UDP port 53");

    initSocket();//初始化SOCKET

    //载入域名表
    initDomainList(&domainList, CACHE_CAPACITY);
    loadDomainList(&domainList, staticDataPath, debugLevel);

    //clients初始化
    int i = 0;
    for (i = 0; i < MAX_PTHREAD / 2; ++i) {
        clients[i].isOccupied = UNOCCUPIED;
        clients[i].id = i;
        initStr(&clients[i].query);
    }

    //servers初始化
    for (i = 0; i < MAX_PTHREAD / 2; ++i) {
        servers[i].isOccupied = UNOCCUPIED;
        initStr(&servers[i].response);
    }

    pthread_t refreshCache;
    pthread_t client;
    pthread_t server;
    pthread_mutex_init(&modifyCache, NULL);
    pthread_mutex_init(&modifyTotalQueryNum, NULL);
    pthread_mutex_init(&output, NULL);

    pthread_create(&refreshCache, NULL, cacheRefresh, NULL);//刷新动态记录进程
    pthread_create(&client, NULL, recvFromClient, NULL);//处理客户端数据进程
    pthread_create(&server, NULL, recvFromServer, NULL);//处理服务器数据进程

    pthread_join(refreshCache, NULL);
    pthread_join(client, NULL);
    pthread_join(server, NULL);

    for (i = 0; i < MAX_PTHREAD / 2; ++i)
        deleteStr(&clients[i].query);

    for (i = 0; i < MAX_PTHREAD / 2; ++i)
        deleteStr(&servers[i].response);

    deleteDomainList(&domainList);
    shutdown(clientSocket, 2);
    shutdown(serverSocket, 2);
    WSACleanup();
    return 0;
}

//处理命令行参数
void initDnsrelay(int argc, char* argv[]) {
    int n[4];
    if (argc >= 2) {
        if (strcmp(argv[1], "-d") == 0) {
            debugLevel = 1;
            if (argc >= 3) {
                if (sscanf(argv[2], "%d.%d.%d.%d", &n[0], &n[1], &n[2], &n[3]) == 4)
                    dnsIp = argv[2];
                else {
                    printf("IP address format error.\n");
                    exit(0);
                }
                if (argc >= 4)
                    staticDataPath = argv[3];
            }
        }
        else if (strcmp(argv[1], "-dd") == 0) {
            debugLevel = 2;
            if (argc >= 3) {
                if (sscanf(argv[2], "%d.%d.%d.%d", &n[0], &n[1], &n[2], &n[3]) == 4)
                    dnsIp = argv[2];
                else {
                    printf("IP address format error.\n");
                    exit(0);
                }
                if (argc >= 4)
                    staticDataPath = argv[3];
            }
        }
        else {
            if (sscanf(argv[1], "%d.%d.%d.%d", &n[0], &n[1], &n[2], &n[3]) == 4)
                dnsIp = argv[1];
            else {
                printf("IP address format error.\n");
                exit(0);
            }
            if (argc >= 3)
                staticDataPath = argv[2];
        }
    }
}

//初始化SOCKET
void initSocket() {
    //初始化套接字库
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsaData) != 0) {
        printf("...WSAStartup failed");
        exit(-1);
    }

    // 绑定本地信息
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&localAdd, 0, sizeof(SOCKADDR_IN));
    localAdd.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    localAdd.sin_family = AF_INET;
    localAdd.sin_port = htons(53);
    if (bind(clientSocket, (SOCKADDR*)&localAdd, sizeof(SOCKADDR)) != 0) {
        printf("...bind failed");
        exit(-1);
    }
    printf("...OK\n");

    //dns服务器信息
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&dnsAdd, 0, sizeof(SOCKADDR_IN));
    dnsAdd.sin_addr.S_un.S_addr = inet_addr(dnsIp);
    dnsAdd.sin_family = AF_INET;
    dnsAdd.sin_port = htons(53);

}

//刷新动态记录
void* cacheRefresh(void* arg)
{
    while (1) {
        Sleep(REFRESH_TIME);
        pthread_mutex_lock(&output);
        printf("Begin refresh cache");
        pthread_mutex_lock(&modifyCache);
        refreshDomianList(&domainList);
        pthread_mutex_unlock(&modifyCache);
        printf("...OK\n");
        pthread_mutex_unlock(&output);
    }
}


//接受客户端数据
void* recvFromClient(void* arg)
{
    while (1) {
        char string[MESSAGE_MAX_LENGTH];
        struct sockaddr_in sockaddr;
        int len = sizeof(SOCKADDR);
        int length = recvfrom(clientSocket, string, MESSAGE_MAX_LENGTH, 0, (SOCKADDR*)&sockaddr, &len);
        if (length > 16) {
            //等待可使用Client的线程
            struct Client* pClient = NULL;
            for (int i = 0; i < MAX_PTHREAD / 2; i++) {
                if (clients[i].isOccupied == UNOCCUPIED) {
                    clients[i].isOccupied = OCCUPIED;
                    pClient = &clients[i];
                    break;
                }
                if (i == MAX_PTHREAD / 2 - 1) {
                    Sleep(1);
                    i = 0;
                }
            }
            setStr(&pClient->query, string, length);
            pClient->clientAdd = sockaddr;

            //创建新线程处理query
            pthread_create(&(pClient->tid), NULL, processQuery, pClient);
            pthread_detach(pClient->tid);
        }
        else {
            pthread_mutex_lock(&output);
            printf("recvfrom() Error #%d\n", GetLastError());
            pthread_mutex_unlock(&output);
        }
    }
    return NULL;
}

//接受dns服务器数据
void* recvFromServer(void* arg)
{
    while (1) {
        char string[MESSAGE_MAX_LENGTH];
        struct sockaddr_in sockaddr;
        while (!allowToReceive) {
            Sleep(1);
        }
        int len = sizeof(SOCKADDR);
        int length = recvfrom(serverSocket, string, MESSAGE_MAX_LENGTH, 0, (SOCKADDR*)&sockaddr, &len);
        if (length > 16) {

            //等待可使用的Server线程
            struct Server* pServer = NULL;
            for (int i = 0; i < MAX_PTHREAD / 2; i++) {
                if (servers[i].isOccupied == UNOCCUPIED) {
                    servers[i].isOccupied = OCCUPIED;
                    pServer = &servers[i];
                    break;
                }
                if (i == MAX_PTHREAD / 2 - 1) {
                    Sleep(1);
                    i = 0;
                }
            }
            setStr(&pServer->response, string, length);

            //创建新线程处理response
            pthread_create(&(pServer->tid), NULL, processResponse, pServer);
            pthread_detach(pServer->tid);
        }
        else {
            pthread_mutex_lock(&output);
            printf("recvfrom() Error #%d\n", GetLastError());
            pthread_mutex_unlock(&output);
        }
    }
}

//处理dns服务器数据
void* processResponse(void* arg) {
    struct Server* pServer = (struct Server*)arg;

    pthread_mutex_lock(&output);
    outputRecvInfo(dnsIp, 53, &pServer->response);
    pthread_mutex_unlock(&output);

    short id = getId(&pServer->response);//获取id，依据id确定客户
    setId(&pServer->response, clients[id].originId);//修改为原始id
    char ip[20];
    inet_ntop(AF_INET, (void*)&clients[id].clientAdd.sin_addr, ip, 16);

    pthread_mutex_lock(&output);
    outputSendInfo(ip, ntohs(clients[id].clientAdd.sin_port), &pServer->response, id, 0);
    pthread_mutex_unlock(&output);

    int length = sendto(clientSocket, pServer->response.string, strLength(&pServer->response), 0, (SOCKADDR*)&clients[id].clientAdd, sizeof(SOCKADDR_IN));
    if (length == -1) {
        pthread_mutex_lock(&output);
        printf("sendto() Error #%d\n", GetLastError());
        pthread_mutex_unlock(&output);

        clients[id].isOccupied = UNOCCUPIED;//解除client线程使用权限
        pServer->isOccupied = UNOCCUPIED;//解除server线程使用权限
        return NULL;
    }
    clients[id].isOccupied = UNOCCUPIED;//解除client线程使用权限

    //保存记录
    Domain domain;
    initDomain(&domain);
    int isNeededToSave = saveRdata(&domain, &pServer->response);//仅保存RDATA字段

    pthread_mutex_lock(&modifyCache);
    if (isNeededToSave == 1 && addDomain(&domainList, &domain) == 0) {
        Domain* pDomain = findDomain(&domainList, &domain.name);
        saveRdata(pDomain, &pServer->response);
    }
    pthread_mutex_unlock(&modifyCache);

    pServer->isOccupied = UNOCCUPIED;// 解除server线程使用权限
    deleteDomain(&domain);
    return NULL;
}


//处理客户端数据
void* processQuery(void* arg) {
    struct Client* pClient = (struct Client*)arg;
    Str domainName;
    initStr(&domainName);
    int type;
    char ip[20];
    inet_ntop(AF_INET, (void*)&pClient->clientAdd.sin_addr, ip, 16);

    pthread_mutex_lock(&output);
    outputRecvInfo(ip, ntohs(pClient->clientAdd.sin_port), &pClient->query);
    pthread_mutex_unlock(&output);

    if ((type = getQnameAndType(&domainName, &pClient->query)) == 0) {
        //无法获得域名
        setQr(&pClient->query, 1);//QR置1
        setRcode(&pClient->query, 1);//返回FOEMAT ERROR

        pthread_mutex_lock(&output);
        outputSendInfo(ip, ntohs(pClient->clientAdd.sin_port), &pClient->query, -1, 0);
        pthread_mutex_unlock(&output);

        int length = sendto(clientSocket, pClient->query.string, strLength(&pClient->query), 0, (SOCKADDR*)&pClient->clientAdd, sizeof(SOCKADDR_IN));
        if (length == -1) {
            pthread_mutex_lock(&output);
            printf("sendto() Error #%d\n", GetLastError());
            pthread_mutex_unlock(&output);
        }
        pClient->isOccupied = UNOCCUPIED;//解除client使用权限
        deleteStr(&domainName);
        return NULL;
    }

    //查询域名记录
    pthread_mutex_lock(&modifyCache);
    Domain* pDomain = findDomain(&domainList, &domainName);
    pthread_mutex_unlock(&modifyCache);

    if (pDomain && (pDomain->isAvailable == 0 || getNumOf(pDomain, type) != -1)) {
        //在本地库找到域名
        if (pDomain->isAvailable == 0) {
            //域名ip不允许访问
            setQr(&pClient->query, 1);//QR置1
            setRcode(&pClient->query, 3);//返回NAME ERROR
        }
        else {
            //存在查询类型的缓存记录
            mergeAnswer(&pClient->query, pDomain, type);//连接answer
            setQr(&pClient->query, 1);//QR置1
            setAncount(&pClient->query, (short)getNumOf(pDomain, type));//修改ANCOUNT
            if (strLength(&pClient->query) > MESSAGE_MAX_LENGTH)//报文超长
                setTc(&pClient->query, 1);//TC置1
        }

        pthread_mutex_lock(&output);
        outputSimpleInfo(ip, pDomain->isStatic, &domainName, type);
        outputSendInfo(ip, ntohs(pClient->clientAdd.sin_port), &pClient->query, -1, 0);
        pthread_mutex_unlock(&output);

        int length = sendto(clientSocket, pClient->query.string, strLength(&pClient->query), 0, (SOCKADDR*)&pClient->clientAdd, sizeof(SOCKADDR_IN));
        if (length == -1) {
            pthread_mutex_lock(&output);
            printf("sendto() Error #%d\n", GetLastError());
            pthread_mutex_unlock(&output);
        }
    }
    else {
        //在本地库未找到域名,发送给dns服务器
        pClient->originId = getId(&pClient->query);//保存原始id
        setId(&pClient->query, pClient->id);//修改为新id

        pthread_mutex_lock(&output);
        outputSimpleInfo(ip, 0, &domainName, type);
        outputSendInfo(dnsIp, 53, &pClient->query, pClient->originId, 0);
        pthread_mutex_unlock(&output);

        int length = sendto(serverSocket, pClient->query.string, strLength(&pClient->query), 0, (SOCKADDR*)&dnsAdd, sizeof(SOCKADDR_IN));
        if (length == -1) {
            pthread_mutex_lock(&output);
            printf("sendto() Error #%d\n", GetLastError());
            pthread_mutex_unlock(&output);
            pClient->isOccupied = UNOCCUPIED;
            deleteStr(&domainName);
            return NULL;
        }

        allowToReceive = 1;
        int i;
        for (i = 0; i < OVERTIME && pClient->isOccupied == OCCUPIED; i += SLEEP_INTERVAL) {
            //周期性判断是否超时
            Sleep(SLEEP_INTERVAL);
        }
    }

    pClient->isOccupied = UNOCCUPIED;//解除client线程使用权限 
    deleteStr(&domainName);
    return NULL;
}

void outputRecvInfo(const char* recvIp, const int recvPort, const Str* pOriginMessage) {
    //输出接受数据信息
    if (debugLevel == 2) {
        printf("RECV from %s:%d (%d bytes)  ", recvIp, recvPort, pOriginMessage->length);
        for (int i = 0; i < pOriginMessage->length; i++) {
            printf("%02hx ", (short)pOriginMessage->string[i] & 0x00ff);
        }
        printf("\n        ID %04hx, QR %d, OPCODE %d,AA %d, TC %d, RD %d, RA %d, Z %d, RCODE %d\n",
            getId(pOriginMessage), getQr(pOriginMessage), getOpcode(pOriginMessage), getAa(pOriginMessage), getTc(pOriginMessage), getRd(pOriginMessage),
            getRa(pOriginMessage), 0, getRcode(pOriginMessage));
        printf("        QDCOUNT %d, ANCOUNT %d, NSCOUNT %d,ARCOUNT %d\n",
            getQdcount(pOriginMessage), getAncount(pOriginMessage), getNscount(pOriginMessage), getArcount(pOriginMessage));
    }
}

void outputSimpleInfo(const char* recvIp, const int isStatic, const Str* domainName, const int type) {
    //输出简单调试信息
    if (debugLevel != 0) {
        time_t timep;
        time(&timep);
        struct tm* now;
        now = gmtime(&timep);
        if (isStatic && type == 1) {
            printf("%8d:* ", queryNum);
        }
        else {
            printf("%8d:  ", queryNum);
        }
        printf("%d-%02d-%02d %02d:%02d:%02d  ", 1900 + now->tm_year, 1 + now->tm_mon, now->tm_mday, 8 + now->tm_hour, now->tm_min, now->tm_sec);

        printf("Client %-16s", recvIp);
        if (type != 1 || debugLevel == 2) {
            printf("%.*s, TYPE %d\n", domainName->length, domainName->string, type);
        }
        else {
            printf("%.*s\n", domainName->length, domainName->string);
        }
        pthread_mutex_lock(&modifyTotalQueryNum);
        queryNum++;
        pthread_mutex_unlock(&modifyTotalQueryNum);
    }
}

void outputSendInfo(const char* sendIp, const int sendPort, const Str* pNewMessage, const int oldId, const int isAddAnswer) {
    //输出发送数据信息
    if (debugLevel == 2) {
        if (isAddAnswer || oldId == -1) {
            printf("SEND to %s:%d (%d bytes)  ", sendIp, sendPort, pNewMessage->length);
            for (int i = 0; i < pNewMessage->length; i++) {
                printf("%02hx ", (short)pNewMessage->string[i] & 0x00ff);
            }
            printf("\n        ID %04hx, QR %d, OPCODE %d,AA %d, TC %d, RD %d, RA %d, Z %d, RCODE %d\n",
                getId(pNewMessage), getQr(pNewMessage), getOpcode(pNewMessage), getAa(pNewMessage), getTc(pNewMessage), getRd(pNewMessage),
                getRa(pNewMessage), 0, getRcode(pNewMessage));
            printf("        QDCOUNT %d, ANCOUNT %d, NSCOUNT %d,ARCOUNT %d\n",
                getQdcount(pNewMessage), getAncount(pNewMessage), getNscount(pNewMessage), getArcount(pNewMessage));
        }
        else {
            printf("SEND to %s:%d (%d bytes) [ID %04x->%04x]\n", sendIp, sendPort, pNewMessage->length, oldId, getId(pNewMessage));
        }
    }
}