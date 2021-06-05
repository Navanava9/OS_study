#include <windows.h>
#include <stdio.h>

void cpu()
{
    // ��ʼ��ʱ
    ULONGLONG qwStartTime = GetTickCount64();

    // cpu ��æ
    for (int i = 0; i < 2000; i++)
        for (int k = 0; k < 2000; k++)
            ;

    // ��ʱ����
    ULONGLONG qwElapseTime = GetTickCount64() - qwStartTime;

    printf("cpu:%lld\n", qwElapseTime);
}

void io()
{
    // �½�һ���ļ�
    HANDLE hFile = CreateFileA(
        "abc.txt",             // name of the write
        GENERIC_WRITE,         // open for writing
        0,                     // do not share
        NULL,                  // default security
        CREATE_ALWAYS,         // create new file only
        FILE_ATTRIBUTE_NORMAL, // normal file
        NULL);                 // no attr. template

    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("CreateFile failed\n");
        return;
    }

    // ��ʼ��ʱ
    ULONGLONG qwStartTime = GetTickCount64();

    // ���ļ���д��һЩ���ݣ����ַ��� DataBuffer д�� 10000 ��
    char DataBuffer[] = "1234567890";

    DWORD dwBytesToWrite = (DWORD)strlen(DataBuffer);
    DWORD dwBytesWritten = 0;

    for (int i = 0; i < 1000; i++)
    {
        BOOL bErrorFlag = WriteFile(
            hFile,           // open file handle
            DataBuffer,      // start of data to write
            dwBytesToWrite,  // number of bytes to write
            &dwBytesWritten, // number of bytes that were written
            NULL);           // no overlapped structure

        if (FALSE == bErrorFlag)
        {
            printf("Terminal failure: Unable to write to file.\n");
            return;
        }
    }
    // �ر��ļ�
    CloseHandle(hFile);

    // ��ʱ����
    ULONGLONG qwElapseTime = GetTickCount64() - qwStartTime;

    printf("io:%lld\n", qwElapseTime);
}

int main(void)
{
    // ��ʼ��ʱ
    ULONGLONG qwStartTime = GetTickCount64();

    cpu();
    io();

    // ��ʱ����
    ULONGLONG qwElapseTime = GetTickCount64() - qwStartTime;

    printf("main:%lld\n", qwElapseTime);

    return 0;
}