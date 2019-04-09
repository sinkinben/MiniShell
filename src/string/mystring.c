#include <string.h>
//make spcace into 0x0
void formatter(char *buf)
{
    int i = 0;
    for (i = 0; i < strlen(buf) - 1; i++)
    {
        if (buf[i] == ' ' && buf[i + 1] == ' ')
        {
            int j = i;
            for (j = i; j < strlen(buf) - 1; j++)
                buf[j] = buf[j + 1];
            buf[j] = 0;
            i--;
        }
    }
    int len = strlen(buf);
    for (i = 0; i < len; i++)
    {
        if (buf[i] == ' ')
            buf[i] = 0;
    }
}

int make_argv(char *buf, int buff_size, char *argv[], int argv_size)
{
    formatter(buf);
    
    int argv_num = 0;
    int i;
    argv[argv_num++] = buf;
    for (i = 0; i < buff_size - 1; i++)
    {
        if (buf[i] == 0 && buf[i + 1] != 0)
        {
            argv[argv_num++] = (buf + i + 1);
            if (argv_num == argv_size)
                break;
        }
    }
    return argv_num;
}
