#include <windows.h>
#include <stdio.h>

void now(char *msg)
{
    SYSTEMTIME time;
    GetLocalTime(&time);
    printf("[ %02d:%02d:%02d.%03d ] %s\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, msg);
}

VOID CALLBACK Timer1Routine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
    now("Timer1");
}

VOID CALLBACK Timer2Routine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
    now("Timer2");
}

int main()
{
    // Create the timer queue.
    HANDLE hTimerQueue = CreateTimerQueue();
    if (NULL == hTimerQueue)
    {
        printf("CreateTimerQueue failed (%d)\n", GetLastError());
        return 2;
    }

    // 创建一个定时器（首次触发时间：2 秒钟以后；触发周期：1 秒钟）
    HANDLE hDummy;
    if (!CreateTimerQueueTimer(
            &hDummy,       // 要返回的定时器对象的句柄
            hTimerQueue,   // 定时器队列对象的句柄
            Timer1Routine, // 回调函数
            0,             // 回调函数的参数
            2000,          // 首次触发时间
            1000,          // 触发周期
            0))            // 未指定什么了不起的标记
    {
        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
        return 3;
    }

    // 再创建一个定时器（首次触发时间：3 秒钟以后；触发周期：2 秒钟）
    if (!CreateTimerQueueTimer(
            &hDummy,       // 要返回的定时器对象的句柄
            hTimerQueue,   // 定时器队列对象的句柄
            Timer2Routine, // 回调函数
            0,             // 回调函数的参数
            3000,          // 首次触发时间
            2000,          // 触发周期
            0))            // 未指定什么了不起的标记
    {
        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
        return 3;
    }

    Sleep(30 * 1000); // 30 秒

    // Delete all timers in the timer queue.
    if (!DeleteTimerQueueEx(
            hTimerQueue,
            INVALID_HANDLE_VALUE // 等待所有的回调函数完成
            ))
        printf("DeleteTimerQueue failed (%d)\n", GetLastError());

    return 0;
}