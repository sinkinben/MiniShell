#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
static char buf[256] = {0};
int main(int argc, char *argv[])
{
    puts("mycp");
    if (argc == 1)
    {
        printf("mycp: missing file operand...\n");
        exit(-1);
    }
    if (argc == 2)
    {
        printf("mycp: missing dst file after '%s'...\n", argv[1]);
        exit(-1);
    }
    printf("src is %s\n", argv[1]);
    printf("dst is %s\n", argv[2]);
    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1)
    {
        printf("mycp: No such file '%s'...\n", argv[1]);
        exit(-1);
    }
    int dst_fd = open(argv[2], O_RDWR | O_CREAT);
    int len = read(src_fd, buf, 255);
    while (len != 0 && len != -1)
    {
        write(dst_fd, buf, len);
        memset(buf, 0, sizeof(buf));
        len = read(src_fd, buf, 255);
    }
    close(src_fd);
    close(dst_fd);
    return 0;
}