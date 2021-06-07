#include <stdio.h>
#include <windows.h>

__int64 FileTimeToQuadWord(PFILETIME pft)
{
    // Int64ShllMod32 ��һ���޷��ŵ�λ���߼���������λ
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

    // ���������ӽ���
    BOOL h1 = CreateProcessA(NULL, "child1.exe", NULL, NULL, 0, NORMAL_PRIORITY_CLASS, NULL, NULL, &startup_info1, &process_info1);
    BOOL h2 = CreateProcessA(NULL, "child2.exe", NULL, NULL, 0, NORMAL_PRIORITY_CLASS, NULL, NULL, &startup_info2, &process_info2);

    // ��ʱ��ʼ
    GetThreadTimes(h1, &ftDummy, &ftDummy, &ftKernelTimeStart, &ftUserTimeStart);

    // �����̽��Լ�����ֱ�������ӽ��̶���ֹ
    WaitForSingleObject(process_info1.hThread, INFINITE);
    WaitForSingleObject(process_info2.hThread, INFINITE);

    // ��ʱ����
    GetThreadTimes(h1, &ftDummy, &ftDummy, &ftKernelTimeEnd, &ftUserTimeEnd);

    // ������ʱ��
    qwKernelTimeElapsed = FileTimeToQuadWord(&ftKernelTimeEnd) - FileTimeToQuadWord(&ftKernelTimeStart);
    qwUserTimeElapsed = FileTimeToQuadWord(&ftUserTimeEnd) - FileTimeToQuadWord(&ftUserTimeStart);
    qwTotalTimeElapsed = qwKernelTimeElapsed + qwUserTimeElapsed;

    // ��ӡʱ��
    printf("totalElapsed is :%lld", qwTotalTimeElapsed);

    return 0;
}