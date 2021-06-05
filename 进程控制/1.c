#include <windows.h>
#include <stdio.h>

int main(void)
{
    STARTUPINFOA startup_info;
    PROCESS_INFORMATION process_info;
    memset(&startup_info, 0, sizeof(startup_info)); 
   
    BOOL ret = CreateProcessA(
        NULL,
        "c:\\windows\\system32\\mspaint.exe",
        NULL,
        NULL,
        0,
        NORMAL_PRIORITY_CLASS,
        NULL,
        NULL,
        &startup_info,
        &process_info);

    if (ret)
        printf("CreateProcess OK.\n");
    else
        printf("CreateProcess Failed\n");

    printf("CurrentProcessId=%d\n", GetCurrentProcessId());
    printf("hProcess=%d\n", process_info.hProcess);
    printf("hThread=%d\n", process_info.hThread);
    printf("dwProcessId=%d\n", process_info.dwProcessId);
    printf("dwThreadId=%d\n", process_info.dwThreadId);

    return 0;
}