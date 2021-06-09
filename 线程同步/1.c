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

    // ����һ����ʱ�����״δ���ʱ�䣺2 �����Ժ󣻴������ڣ�1 ���ӣ�
    HANDLE hDummy;
    if (!CreateTimerQueueTimer(
            &hDummy,       // Ҫ���صĶ�ʱ������ľ��
            hTimerQueue,   // ��ʱ�����ж���ľ��
            Timer1Routine, // �ص�����
            0,             // �ص������Ĳ���
            2000,          // �״δ���ʱ��
            1000,          // ��������
            0))            // δָ��ʲô�˲���ı��
    {
        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
        return 3;
    }

    // �ٴ���һ����ʱ�����״δ���ʱ�䣺3 �����Ժ󣻴������ڣ�2 ���ӣ�
    if (!CreateTimerQueueTimer(
            &hDummy,       // Ҫ���صĶ�ʱ������ľ��
            hTimerQueue,   // ��ʱ�����ж���ľ��
            Timer2Routine, // �ص�����
            0,             // �ص������Ĳ���
            3000,          // �״δ���ʱ��
            2000,          // ��������
            0))            // δָ��ʲô�˲���ı��
    {
        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
        return 3;
    }

    Sleep(30 * 1000); // 30 ��

    // Delete all timers in the timer queue.
    if (!DeleteTimerQueueEx(
            hTimerQueue,
            INVALID_HANDLE_VALUE // �ȴ����еĻص��������
            ))
        printf("DeleteTimerQueue failed (%d)\n", GetLastError());

    return 0;
}