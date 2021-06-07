#include <stdio.h>
#include <windows.h>

__int64 FileTimeToQuadWord(PFILETIME pft)
{
    // Int64ShllMod32 将一个无符号的位数逻辑左移若干位
    return (Int64ShllMod32(pft->dwHighDateTime, 32) | pft->dwLowDateTime);
}

int main(void)
{
    FILETIME ftKernelTimeStart, ftKernelTimeEnd;
    FILETIME ftUserTimeStart, ftUserTimeEnd;
    FILETIME ftDummy;
    __int64 qwKernelTimeElapsed, qwUserTimeElapsed, qwTotalTimeElapsed;

    STARTUPINFOA startup_info1, startup_info2;
    PROCESS_INFORMATION process_info1, process_info2;
    memset(&startup_info1, 0, sizeof(startup_info1));
    memset(&startup_info2, 0, sizeof(startup_info2));

    // 创建两个子进程
    BOOL h1 = CreateProcessA(NULL, "child1.exe", NULL, NULL, 0, NORMAL_PRIORITY_CLASS, NULL, NULL, &startup_info1, &process_info1);
    BOOL h2 = CreateProcessA(NULL, "child2.exe", NULL, NULL, 0, NORMAL_PRIORITY_CLASS, NULL, NULL, &startup_info2, &process_info2);

    // 计时开始
    GetThreadTimes(h1, &ftDummy, &ftDummy, &ftKernelTimeStart, &ftUserTimeStart);

    // 父进程将自己挂起，直到两个子进程都终止
    WaitForSingleObject(process_info1.hThread, INFINITE);
    WaitForSingleObject(process_info2.hThread, INFINITE);

    // 计时结束
    GetThreadTimes(h1, &ftDummy, &ftDummy, &ftKernelTimeEnd, &ftUserTimeEnd);

    // 计算总时间
    qwKernelTimeElapsed = FileTimeToQuadWord(&ftKernelTimeEnd) - FileTimeToQuadWord(&ftKernelTimeStart);
    qwUserTimeElapsed = FileTimeToQuadWord(&ftUserTimeEnd) - FileTimeToQuadWord(&ftUserTimeStart);
    qwTotalTimeElapsed = qwKernelTimeElapsed + qwUserTimeElapsed;

    // 打印时间
    printf("totalElapsed is :%lld", qwTotalTimeElapsed);

    return 0;
}