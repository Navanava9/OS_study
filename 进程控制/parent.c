#include <stdio.h>
#include <windows.h>

int main(void)
{
    STARTUPINFOA startup_info1, startup_info2;
    PROCESS_INFORMATION process_info1, process_info2;
    memset(&startup_info1, 0, sizeof(startup_info1));
    memset(&startup_info2, 0, sizeof(startup_info2));

    // ���������ӽ���
    BOOL h1 = CreateProcessA(NULL, "child1.exe", NULL, NULL, 0, NORMAL_PRIORITY_CLASS, NULL, NULL, &startup_info1, &process_info1);
    BOOL h2 = CreateProcessA(NULL, "child2.exe", NULL, NULL, 0, NORMAL_PRIORITY_CLASS, NULL, NULL, &startup_info2, &process_info2);

    ULONGLONG qwStartTime = GetTickCount();

    // �����̽��Լ�����ֱ�������ӽ��̶���ֹ
    WaitForSingleObject(process_info1.hThread, INFINITE);
    WaitForSingleObject(process_info2.hThread, INFINITE);

    // ��ʱ����
    ULONGLONG qwElapseTime = GetTickCount() - qwStartTime;

    printf("main:%lld\n", qwElapseTime);

    return 0;
}