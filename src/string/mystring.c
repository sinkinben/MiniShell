#include <string.h>
#include <assert.h>
void insert_space(char buf[], int buf_size, int pos)
{
    int i = pos;
    int len = strlen(buf);
    assert((len + 1) < buf_size);
    for (i = len; i > pos; i--)
        buf[i] = buf[i - 1];
    buf[i] = ' ';
}
//merge space into one
//make sure that >, >>, <, <<, | have space around them
void formatter(char *buf, int buf_size)
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

    for (i = 1; i < strlen(buf); i++)
    {
        if (buf[i] == '>' && buf[i - 1] != '>' && buf[i - 1] != ' ')
            insert_space(buf, buf_size, i);
        else if (buf[i] == '<' && buf[i - 1] != '<' && buf[i - 1] != ' ')
            insert_space(buf, buf_size, i);
        else if (buf[i] == '|' && buf[i - 1] != ' ')
            insert_space(buf, buf_size, i);
    }

    for (i = 0; i < strlen(buf) - 1; i++)
    {
        if (buf[i] == '>' && buf[i + 1] != '>' && buf[i + 1] != ' ')
            insert_space(buf, buf_size, i + 1);
        else if (buf[i] == '<' && buf[i + 1] != '<' && buf[i + 1] != ' ')
            insert_space(buf, buf_size, i + 1);
        else if (buf[i] == '|' && buf[i + 1] != ' ')
            insert_space(buf, buf_size, i + 1);
    }
    int len = strlen(buf);
    if (buf[len - 1] == ' ')
        buf[len - 1] = 0;
}

int make_argv(char *name, char args[], char *argv[], int argv_size)
{
    int i = 0;
    argv[i++] = name;
    char *p = strtok(args, " ");
    while (p)
    {
        argv[i++] = p;
        if (i == argv_size)
            break;
        p = strtok(NULL, " ");
    }
    return i;
}
