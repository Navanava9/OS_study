#include <windows.h>
#include <stdio.h>

DWORD WINAPI tf1(void *param) // 线程1
{
    char ch = (char)param;
    for (int i = 0; i < 10; i++)
        printf("%c", ch);
    return 0;
}

DWORD WINAPI tf2(void *param) // 线程2
{
    char *str = (char *)param;
    for (int i = 0; i < 10; i++)
        printf("%s", str);
    return 0;
}

void main()
{
    tf1((void *)'1');

    // 创建2个线程并发执行，并且通过正常返回结束线程；
    CreateThread(0, 0, tf1, (void *)'2', 0, 0);
    CreateThread(0, 0, tf1, (void *)'3', 0, 0);

    char *str = "abc";

    CreateThread(0, 0, tf2, (void *)str, 0, 0);

    getc(stdin);
}
