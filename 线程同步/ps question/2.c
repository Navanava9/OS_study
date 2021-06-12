//1生产者 2消费者 4缓冲区
#include <stdio.h>
#include <process.h>
#include <windows.h>

#define END_PRODUCE_NUMBER 8 //生产产品个数
#define BUFFER_SIZE 4
//缓冲区个数
int g_Buffer[BUFFER_SIZE]; //缓冲池
int g_i, g_j;
//信号量与关键段
CRITICAL_SECTION g_cs;
HANDLE g_hSemaphoreBufferEmpty, g_hSemaphoreBufferFull;
//生产者线程函数
DWORD WINAPI ProducerThreadFun(PVOID pM)
{
    for (int i = 1; i <= END_PRODUCE_NUMBER; i++)
    {
        //等待有空的缓冲区出现
        WaitForSingleObject(g_hSemaphoreBufferEmpty, INFINITE);

        //互斥的访问缓冲区
        EnterCriticalSection(&g_cs);
        g_Buffer[g_i] = i;
        printf("生产者在缓冲池第%d个缓冲区中投放数据%d\n", g_i, g_Buffer[g_i]);
        g_i = (g_i + 1) % BUFFER_SIZE;
        LeaveCriticalSection(&g_cs);

        //通知消费者有新数据了
        ReleaseSemaphore(g_hSemaphoreBufferFull, 1, NULL);
    }
    printf("生产者完成任务，线程结束运行\n");
    return 0;
}
//消费者线程函数
DWORD WINAPI ConsumerThreadFun(PVOID pM)
{
    while (1)
    {
        //等待非空的缓冲区出现
        WaitForSingleObject(g_hSemaphoreBufferFull, INFINITE);

        //互斥的访问缓冲区
        EnterCriticalSection(&g_cs);
        printf("  编号为%d的消费者从缓冲池中第%d个缓冲区取出数据%d\n", GetCurrentThreadId(), g_j, g_Buffer[g_j]);
        if (g_Buffer[g_j] == END_PRODUCE_NUMBER) //结束标志
        {
            LeaveCriticalSection(&g_cs);
            //通知其它消费者有新数据了(结束标志)
            ReleaseSemaphore(g_hSemaphoreBufferFull, 1, NULL);
            break;
        }
        g_j = (g_j + 1) % BUFFER_SIZE;
        LeaveCriticalSection(&g_cs);

        Sleep(50); //some other work to do

        ReleaseSemaphore(g_hSemaphoreBufferEmpty, 1, NULL);
    }
    printf("  编号为%d的消费者收到通知，线程结束运行\n", GetCurrentThreadId());
    return 0;
}
int main()
{
    InitializeCriticalSection(&g_cs);
    //初始化信号量,一个记录有产品的缓冲区个数,另一个记录空缓冲区个数.
    g_hSemaphoreBufferEmpty = CreateSemaphore(NULL, 4, 4, NULL);
    g_hSemaphoreBufferFull = CreateSemaphore(NULL, 0, 4, NULL);
    g_i = 0;
    g_j = 0;
    memset(g_Buffer, 0, sizeof(g_Buffer));

    const int THREADNUM = 3;
    HANDLE hThread[THREADNUM];
    //生产者线程
    hThread[0] = (HANDLE)CreateThread(NULL, 0, ProducerThreadFun, NULL, 0, NULL);
    //消费者线程
    hThread[1] = (HANDLE)CreateThread(NULL, 0, ConsumerThreadFun, NULL, 0, NULL);
    hThread[2] = (HANDLE)CreateThread(NULL, 0, ConsumerThreadFun, NULL, 0, NULL);
    WaitForMultipleObjects(THREADNUM, hThread, TRUE, INFINITE);
    
    for (int i = 0; i < THREADNUM; i++)
        CloseHandle(hThread[i]);

    //销毁信号量和关键段
    CloseHandle(g_hSemaphoreBufferEmpty);
    CloseHandle(g_hSemaphoreBufferFull);
    DeleteCriticalSection(&g_cs);
    return 0;
}