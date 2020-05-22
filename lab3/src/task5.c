#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("start");
    execl("./task1.out"," ","1","2",NULL);
    return 0;
}