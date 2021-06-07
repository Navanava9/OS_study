#include <stdio.h>
#include <windows.h>

DWORD WINAPI tf(void *p)
{
    for (int i = 0; i < 50; i++)
    {
        Sleep(500);
        printf("%d ", i);
    }
    printf("\n");
    return 0;
}

void main()
{
    // 其中第五个参数表明创建的线程初始为挂起状态；
    HANDLE hChildThread = CreateThread(0, 0, tf, 0, CREATE_SUSPENDED, 0);

    printf("Press any key to resume thread\n");
    getc(stdin);
    ResumeThread(hChildThread);

    printf("Main sleep 10000 ms and suspend thread\n");
    Sleep(10000);
    SuspendThread(hChildThread);

    printf("Main sleep 10000 ms and resume thread\n");
    Sleep(10000);
    ResumeThread(hChildThread);

    getc(stdin);
}