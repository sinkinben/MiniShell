#include "shell.h"
#include <assert.h>
#include <errno.h>
#include <signal.h>
#define BUFF_SIZE 512
#define CMDS_LEN 16
static char title[BUFF_SIZE] = ">>"; //shell prompt
static char *line = NULL;            //to read line from stdin
struct passwd *user;                 //get current user from os
cmd_t cmds[CMDS_LEN];
char cwd[BUFF_SIZE];
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

void callback_child(int signo)
{
	pid_t pid;
	int status = 0;
	pid = wait(&status);

	if (WEXITSTATUS(status) ==  EXIT_FAILURE)
	{
		puts(strerror(WEXITSTATUS(status)));
	}
	printf("[%d]+ Done.\n", pid);
}

void ui_mainloop()
{
    char *cmd_str;
    char input_buf[BUFF_SIZE] = {0};
    user = getpwuid(getuid());
    getcwd(cwd, BUFF_SIZE);
    while (true)
    {
        get_title();
        cmd_str = get_line(title);

        if (strcmp(cmd_str, "") == 0)
            continue;

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
        if (cmd_num == 1 && buildin_handler(&cmds[0]) != -1)
            continue;
        else
        {
            pid_t pid = fork();
            if (pid < 0)
            {
                fprintf(stderr, "fork failed\n");
                continue;
            }
            else if (pid == 0)
            {
                external_handler(cmds, cmd_num, cmd_num - 1);
            }
            else
            {
                bool wait_flag = cmds[cmd_num - 1].attr.wait_flag;
                if (wait_flag == WAIT_BKGD)
                {
                    printf("[%d] run in background\n", pid);
					signal(SIGCHLD, callback_child);
                }
                else
                {
					signal(SIGCHLD, SIG_IGN);
                    int status = 0;
                    waitpid(pid, &status, 0);
                    if ((WEXITSTATUS(status)) == EXIT_FAILURE)
                    {
                        puts(strerror(WEXITSTATUS(status)));
                    }
                }
            }
        }
    }
}
