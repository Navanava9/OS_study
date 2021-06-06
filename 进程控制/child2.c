#include <stdio.h>
#include <windows.h>

void io()
{
    // 新建一个文件
    HANDLE hFile = CreateFileA(
        "abc.txt",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("CreateFile failed\n");
        return;
    }
    // 向文件中写入一些数据：将字符串 DataBuffer 写入 10000 次
    char DataBuffer[] = "1234567890";

    DWORD dwBytesToWrite = (DWORD)strlen(DataBuffer);
    DWORD dwBytesWritten = 0;

    for (int i = 0; i < 10000; i++)
    {
        BOOL bErrorFlag = WriteFile(
            hFile,
            DataBuffer,
            dwBytesToWrite,
            &dwBytesWritten,
            NULL);

        if (FALSE == bErrorFlag)
        {
            printf("Terminal failure: Unable to write to file.\n");
            return;
        }
    }
    // 关闭文件
    CloseHandle(hFile);
}

int main(void)
{
    io();

    return 0;
}