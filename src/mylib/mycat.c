#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
static char buf[1024 + 5] = {0};
int main(int argc, char *argv[])
{
    puts("mycat");
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        perror("'%s': no such file...\n");
        exit(EXIT_FAILURE);
    }
    int len = read(fd, buf, 1024);
    while (len != 0 && len != -1)
    {
        printf("%s", buf);
        memset(buf, 0, sizeof(buf));
        len = read(fd, buf, 1024);
    }
    return 0;
}