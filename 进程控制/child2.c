#include <stdio.h>
#include <windows.h>

void io()
{
    // �½�һ���ļ�
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
    // ���ļ���д��һЩ���ݣ����ַ��� DataBuffer д�� 10000 ��
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
    // �ر��ļ�
    CloseHandle(hFile);
}

int main(void)
{
    io();

    return 0;
}