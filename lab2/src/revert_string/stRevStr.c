#include <stdlib.h>
#include <string.h>

extern void RevertString(char* str)
{
    char* temp = (char*)malloc(sizeof(char) * (strlen(str) + 1));
    int i = strlen(str) - 1;
    int w = strlen(str) - 1;
    while (*str != '\0') {
        *(temp + i) = *str;
        str++;
        i--;
    }
    *(temp + w + 1) = '\0';
    strcpy(str - w - 1, temp);
}