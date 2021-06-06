#include<windows.h>
#include<stdio.h>
 
DWORD WINAPI tf1(void *param)
{
   char ch=(char)param;
   for(int i=0;i<500;i++) printf("%c",ch);
   return 0;
}
 
DWORD WINAPI tf2(void *param)
{
   char *str=(char*)param;
   for(int i=0;i<300;i++) printf("%s",str);
   return 0;
}
 
void main()
{
   tf1((void *)'1');
   CreateThread(0,0,tf1,(void*)'2',0,0);
   CreateThread(0,0,tf1,(void*)'3',0,0);
   char *str="abc";
   CreateThread(0,0,tf2,(void*)str,0,0);
   getc(stdin);        
}