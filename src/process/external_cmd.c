#include <unistd.h>
#include <string.h>
#include "process.h"
void external_handler(char *argv[])
{
    char buff[1024] = "";
    int i = 0;
    while (argv[i] != NULL)
        strcat(buff, argv[i++]), strcat(buff, " ");
    execlp("/bin/sh", "sh", "-c", buff, NULL);
}