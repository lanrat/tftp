#include "test.h"

void printBuff(void* buff, unsigned int size)
{
    char* ptr = (char*)buff;
    printf("------------------\n");
    while (size-- > 0)
    {
        printf("[%2X]",*ptr++);
    }
    printf("\n------------------\n");
}
