#include "shell.h"
extern char cwd[];
int make_cmds(char buf[], int buf_size, cmd_t cmds[], int cmds_size)
{
    int i = 0;
    char *name = strtok(buf, " ");
    char *args = strtok(NULL, " ");
    strcpy(cmds[i].name, name);

    while (args)
    {
        if (strcmp(args, "<") == 0)
        {
            cmds[i].attr.redir_stdin = REDIR_IN1;
            args = strtok(NULL, " ");
            strcpy(cmds[i].src_file, args);
        }
        else if (strcmp(args, "<<") == 0)
        {
            cmds[i].attr.redir_stdin = REDIR_IN2;
            args = strtok(NULL, " ");
            strcpy(cmds[i].src_file, args);
        }
        else if (strcmp(args, ">") == 0)
        {
            cmds[i].attr.redir_stdout = REIDR_OUT_CLEAR;
            args = strtok(NULL, " ");
            strcpy(cmds[i].dst_file, args);
        }
        else if (strcmp(args, ">>") == 0)
        {
            cmds[i].attr.redir_stdout = REDIR_OUT_APPEND;
            args = strtok(NULL, " ");
            strcpy(cmds[i].dst_file, args);
        }
        else if (strcmp(args, "&") == 0)
        {
            cmds[i].attr.wait_flag = 1;
        }
        else if (strcmp(args, "|") == 0)
        {
            cmds[i].attr.pipe_flag |= PIPE_IN;
            i++;
            cmds[i].attr.pipe_flag |= PIPE_OUT;
            name = strtok(NULL, " ");
            strcpy(cmds[i].name, name);
        }
        else
        {
            strcat(cmds[i].args, args);
            strcat(cmds[i].args, " ");
        }
        args = strtok(NULL, " ");
    }

    int cmd_num = i + 1;
    int len;
    for (i = 0; i < cmd_num; i++)
    {
        len = strlen(cmds[i].args);
        if (len >= 1 && cmds[i].args[len - 1] == ' ')
            cmds[i].args[len - 1] = 0;
        
    }
    return cmd_num;
}

bool scan_dir(const char *path, const char *target)
{
    bool flag = false;
    DIR *dir;
    struct dirent *dp;
    dir = opendir(path);
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(target, dp->d_name) == 0)
        {
            flag = true;
            break;
        }
    }
    closedir(dir);
    return flag;
}