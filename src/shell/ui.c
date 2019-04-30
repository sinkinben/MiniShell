#include "shell.h"
#define BUFF_SIZE 512
#define CMDS_LEN 16
static char title[BUFF_SIZE] = ">>"; //shell prompt
static char *line = NULL;            //to read line from stdin
struct passwd *user;                 //get current user from os
static cmd_t cmds[CMDS_LEN];
void get_title()
{
    //TODO：title = <user>@<cwd>:
}

char *get_line(char *prompt)
{
    if (line)
    {
        free(line);
        line = NULL;
    }
    line = readline(prompt);
    if (!line)
    {
        printf("\n");
        exit(0);
    }
    if (line && *line)
        add_history(line);
    return line;
}

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

void ui_mainloop()
{
    char *cmd_str;
    char input_buf[BUFF_SIZE] = {0};
    user = getpwuid(getuid());
    while (true)
    {
        get_title();
        cmd_str = get_line(title);
        memset(input_buf, 0, sizeof(input_buf));
        memset(cmds, 0, sizeof(cmds));
        memcpy(input_buf, cmd_str, strlen(cmd_str));

        formatter(input_buf);

        int cmd_num = make_cmds(input_buf, BUFF_SIZE, cmds, CMDS_LEN);

#ifdef DEBUG
        int i = 0;
        for (i = 0; i < cmd_num; i++)
        {
            log("[%s], [%s], [src=%s], [dst=%s]", cmds[i].name, cmds[i].args, cmds[i].src_file, cmds[i].dst_file);
            log("[redin = %d], [redout = %d], [pipe = %d]", cmds[i].attr.redir_stdin, cmds[i].attr.redir_stdout, cmds[i].attr.pipe_flag);
            putchar('\n');
        }
#endif

        
    }
}
