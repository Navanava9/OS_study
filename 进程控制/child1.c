#include <windows.h>
#include <stdio.h>

void cpu()
{
    for (int i = 0; i < 2000; i++)
        for (int k = 0; k < 20000; k++)
            ;
}

int main(void)
{
    cpu();

    return 0;
}