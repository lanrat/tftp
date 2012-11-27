#include "tftp.h"


/* prbly very similar to strncpy but returns the size of the string copied */
size_t charncpy(char *dest, const char *src, size_t n)
{
    size_t i,len;

    for (i = 0; i < n && src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }
    len = i;
    for ( ; i < n; i++)
    {
        dest[i] = '\0';
    }

    return len+1;
}

/* A little function to get the host ordering of a short from a pointer */
u_int16_t getHostOrderShortFromNetwork(void * buff)
{
    u_int16_t data;
    memcpy(&data,buff,sizeof(u_int16_t));
    return ntohs(data);
}

/* A function to get the network ordering of a short from a pointer */
u_int16_t getNetworkOrderShortFromHost(u_int16_t hostshort, void * buff)
{
    u_int16_t data = htons(hostshort);
    if (buff != NULL)
    {
        memcpy(buff,&data,sizeof(u_int16_t));
    }
    return data;
}

