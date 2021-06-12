#include <windows.h>
#include <stdio.h>
#include <string.h>

wchar_t FileName[MAX_PATH] = {0};
HANDLE hFile;

void ShowHelp()
{
    printf("语法:\n");
    printf("    ReadFile -d N\n");
    printf("    ReadFile -v V\n");
    printf("    ReadFile -f F\n");
    printf("项参数:\n");
    printf("    N 物理磁盘号, N = 0, 1, ...\n");
    printf("    V 盘符, V = A:, B:, ...\n");
    printf("    F 文件名, 可以指定完整的文件名，包括盘符、路径和文件名\n");
    printf("举例：\n");
    printf("    ReadFile -d 0\n");
    printf("    ReadFile -v D:\n");
    printf("    ReadFile -f e:\\tmp\\abc.txt\n\n");
}

void ShowSector(char *buf, unsigned __int64 offset)
{
    for (int p = 0;;)
    {
        if (p >= 512)
            break;
        printf("%016llX ", offset);
        for (int i = 0; i < 16; i++)
        {
            if (i == 8)
                printf("- ");
            printf("%02X ", (unsigned __int8)buf[p + i]);
        }
        for (int i = 0; i < 16; i++)
        {
            if (buf[p + i] >= 0x20 && buf[p + i] <= 0x7e)
                printf("%c", buf[p + i]);
            else
                printf(".");
        }
        printf("\n");
        p = p + 16;
        offset = offset + 16;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("参数错误\n");
        ShowHelp();
        return 1;
    }
    // 得到文件名
    if (!strcmp(argv[1], "-d"))
    {
        wsprintfW(FileName, L"\\\\.\\PhysicalDrive%d", atoi(argv[2]));
    }
    else if (!strcmp(argv[1], "-v"))
    {
        wsprintfW(FileName, L"\\\\.\\%c:", argv[2][0]);
    }
    else if (!strcmp(argv[1], "-f"))
    {
        wsprintfW(FileName, L"%S", argv[2]);
    }
    else
    {

        printf("参数错误\n");
        ShowHelp();
        return 1;
    }
    // 打开文件
    hFile = CreateFileW(
        FileName,                           // to open
        GENERIC_READ,                       // no access to the drive
        FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
        NULL,                               // default security attributes
        OPEN_EXISTING,                      // disposition
        0,                                  // file attributes
        NULL);                              // do not copy file attributes
    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("无法打开指定的文件, 注意，访问些文件（比如磁盘和分区）通常需要管理员权限\n");
        return 1;
    }

    char cmd[MAX_PATH];
    char Buffer[512];

    while (1)
    {
        // 显示提示符，等待输入
        printf("\n-");
        gets_s(cmd, MAX_PATH);
        // 退出命令
        if (!strcmp(cmd, "q") || !strcmp(cmd, "Q"))
        {
            break;
        }
        // 显示指定的文件偏移处的数据，，命令格式为 16 进制数字串
        else if (cmd[0] == 'b' || cmd[0] == 's')
        {
            unsigned __int64 off;
            off = _strtoi64(&cmd[2], NULL, 16);
            if (cmd[0] == 'b')
            {
                if (off % 512)
                    off = off / 512 * 512;
            }
            else
            {
                off = off * 512;
            }
            LARGE_INTEGER li = {0};
            li.QuadPart = off;
            LARGE_INTEGER NewFilePointer;
            if (SetFilePointerEx(hFile, li, &NewFilePointer, 0))
            {
                DWORD NumberOfBytesRead;
                if (ReadFile(
                        hFile,
                        Buffer,
                        512,
                        &NumberOfBytesRead,
                        NULL))
                {
                    ShowSector(Buffer, off);
                }
                else
                {
                    printf("\nread failed\n");
                }
            }
            else
            {
                printf("\nseek failed\n");
            }
        }
        else
        {

            if (strcmp(cmd, "?") && strcmp(cmd, "h") && strcmp(cmd, "H"))
            {
                printf("不可识别的命令\n");
                printf("可识别的命令如下：\n");
            }
            printf("q|Q   退出\n");
            printf("?|h|H 显示帮助\n");
            printf("b X   显示文件中的第 X 个字节（byte所在的扇区中的数据， X 为 16 进制数，最长为 16 个字符，比如 1F00 ，12AAC00\n");
            printf("s X   显示文件中的第 X 个扇区（sector，X 的语法同上\n\n");
        }
    }
    CloseHandle(hFile);
    return 0;
}