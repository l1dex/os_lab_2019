#include <stdio.h>
#include <unistd.h>
int main(){
    int pid = fork();
    if(pid == 0){
        printf("Child proccess");
        return 0;
    }else{
        for(;;);
    }
}