#include "shell.h"
#define BUFF_SIZE 512
#define CMDS_LEN 16
static char input[BUFF_SIZE] = {0};  //input buffer
static char title[BUFF_SIZE] = ">>"; //shell prompt
static char *line = NULL;            //to read line from stdin
struct passwd *user;                 //get current user from os
//static cmd_t cmds[CMDS_LEN];
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

void ui_mainloop()
{
    char *str = NULL;
    char *argv[1024] = {0};
    int argv_num = 0;
    const int argv_size = 1024;
    user = getpwuid(getuid());
    while (true)
    {
        get_title();
        str = get_line(title);
        memset(argv, 0, sizeof(argv));
        memset(input, 0, sizeof(input));
        memcpy(input, str, strlen(str));

        argv_num = make_argv(input, BUFF_SIZE, argv, argv_size);

        if (buildin_handler(argv) != -1)
            continue;
        else
        {
            
        }
    }
}