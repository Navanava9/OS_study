#include <windows.h>
#include <stdio.h>
#include <string.h>

wchar_t FileName[MAX_PATH] = {0};
HANDLE hFile;

void ShowHelp()
{
    printf("�﷨:\n");
    printf("    ReadFile -d N\n");
    printf("    ReadFile -v V\n");
    printf("    ReadFile -f F\n");
    printf("�����:\n");
    printf("    N ������̺�, N = 0, 1, ...\n");
    printf("    V �̷�, V = A:, B:, ...\n");
    printf("    F �ļ���, ����ָ���������ļ����������̷���·�����ļ���\n");
    printf("������\n");
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
        printf("��������\n");
        ShowHelp();
        return 1;
    }
    // �õ��ļ���
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

        printf("��������\n");
        ShowHelp();
        return 1;
    }
    // ���ļ�
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
        printf("�޷���ָ�����ļ�, ע�⣬����Щ�ļ���������̺ͷ�����ͨ����Ҫ����ԱȨ��\n");
        return 1;
    }

    char cmd[MAX_PATH];
    char Buffer[512];

    while (1)
    {
        // ��ʾ��ʾ�����ȴ�����
        printf("\n-");
        gets_s(cmd, MAX_PATH);
        // �˳�����
        if (!strcmp(cmd, "q") || !strcmp(cmd, "Q"))
        {
            break;
        }
        // ��ʾָ�����ļ�ƫ�ƴ������ݣ��������ʽΪ 16 �������ִ�
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
                printf("����ʶ�������\n");
                printf("��ʶ����������£�\n");
            }
            printf("q|Q   �˳�\n");
            printf("?|h|H ��ʾ����\n");
            printf("b X   ��ʾ�ļ��еĵ� X ���ֽڣ�byte���ڵ������е����ݣ� X Ϊ 16 ���������Ϊ 16 ���ַ������� 1F00 ��12AAC00\n");
            printf("s X   ��ʾ�ļ��еĵ� X ��������sector��X ���﷨ͬ��\n\n");
        }
    }
    CloseHandle(hFile);
    return 0;
}