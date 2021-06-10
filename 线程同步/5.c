#include <stdio.h>
#include <conio.h>
#include <windows.h>

int main(void)
{
    STARTUPINFOA startup_info1, startup_info2;
    PROCESS_INFORMATION process_info1, process_info2;
    memset(&startup_info1, 0, sizeof(startup_info1));
    memset(&startup_info2, 0, sizeof(startup_info2));

    BOOL h1 = CreateProcessA(NULL, "4.exe", NULL, NULL, 0, NORMAL_PRIORITY_CLASS, NULL, NULL, &startup_info1, &process_info1);

    getc(stdin);

    BOOL h2 = CreateProcessA(NULL, "3.exe", NULL, NULL, 0, NORMAL_PRIORITY_CLASS, NULL, NULL, &startup_info2, &process_info2);

    return 0;
}