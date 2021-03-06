#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <assert.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
void tree(char *dir, int depth);
void formatter(char *buf, int buf_size);
void scan_dir(char *p);
void test1()
{
    char str[BUFSIZ] = "ls -l>file|cat<<sort>>l";
    formatter(str, BUFSIZ);
    printf("[%s]\n", str);
}
void test2()
{
    char buffer[BUFSIZ];
    getcwd(buffer, BUFSIZ);

    puts(buffer);
    tree("/home/sin/desktop/workspace/OSLab/MiniShell", 0);

    memset(buffer, 0, sizeof(buffer));
    getcwd(buffer, BUFSIZ);
    puts(buffer);
}
void test3()
{
    scan_dir("/home/sin/desktop/workspace/OSLab/MiniShell/include/");
}
void test4()
{
    char *p = (getenv("PATH"));
    char new[BUFSIZ] = "PATH=";
    strcat(new, p);
    strcat(new, ":/home");
    if (putenv(new) == -1)
        puts("****");
    puts(p);
    p = getenv("PATH");
    puts(p);
    
}
int main()
{
    test4();
}

void scan_dir(char *p)
{
    DIR *dir;
    struct dirent *dp;
    dir = opendir(p);
    while ((dp = readdir(dir)) != NULL)
    {
        printf("%s\n", dp->d_name);
    }
    closedir(dir);
}
void tree(char *dir, int depth)
{
    int i;
    char cwd_buff[128];
    getcwd(cwd_buff, sizeof(cwd_buff));

    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if ((dp = opendir(dir)) == NULL)
    {
        fprintf(stderr, "open failed\n");
        return;
    }
    chdir(dir);
    while ((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode))
        {
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0 || entry->d_name[0] == '.')
            {
                continue;
            }
            for (i = 0; i < depth - 4; i++)
                printf(" ");
            printf("|");
            for (i = 0; i < 3; i++)
                printf("_");
            printf("%s/\n", entry->d_name);
            tree(entry->d_name, depth + 4);
        }
        else
        {
            printf("|");
            for (i = 0; i < depth - 3; i++)
                printf(" ");
            printf("|");
            for (i = 0; i < 3; i++)
                printf("_");
            printf("%s\n", entry->d_name);
        }
    }
    chdir(cwd_buff); //switch to original dir
    closedir(dp);
}
void insert_space(char buf[], int buf_size, int pos)
{
    int i = pos;
    int len = strlen(buf);
    assert((len + 1) < buf_size);
    for (i = len; i > pos; i--)
        buf[i] = buf[i - 1];
    buf[i] = ' ';
}
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
}
