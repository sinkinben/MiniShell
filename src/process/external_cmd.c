#include "shell.h"
#include <errno.h>
#include <signal.h>
void external_handler(cmd_t cmds[], int cmd_num, int depth)
{
	signal(SIGCHLD, SIG_IGN);
    if (depth == -1)
        exit(EXIT_SUCCESS);

    //let pipe-in cmd wait for pipe-out cmd's input
    cmd_t *cmd = &cmds[depth];
    pid_t pid;
    int fd[2];
    

    char name[CMD_NAME_LEN] = {0};
    char args[CMD_ARGS_LEN] = {0};
    char *argv[CMD_ARGV_LEN] = {0};

    strcpy(name, cmd->name);
    strcpy(args, cmd->args);

    make_argv(name, args, argv, CMD_ARGV_LEN);

    pipe(fd);
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "In external_handler: fork failed\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // if cmds[depth] is first cmd, it has no command before
        // so it do not need to dup, cmd[0] will exec in parent process
        if (depth != 0)
        {
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
        }
        external_handler(cmds, cmd_num, depth - 1);
		exit(EXIT_SUCCESS);
    }
    else
    {
        int status = 0;
        waitpid(pid, &status, 0);
        if ((WEXITSTATUS(status)) == EXIT_FAILURE)
        {
            fprintf(stderr, "In external_handler %d line: wait failed\n", __LINE__);
            puts(strerror(errno));
        }

        // cmd[0] must be exec directly
        if (depth != 0)
        {
            dup2(fd[0], 0);
            close(fd[0]);
            close(fd[1]);
        }

        // >, >>
        if (cmd->attr.redir_stdout)
        {
            int flag = O_CREAT | O_RDWR;
            if (cmd->attr.redir_stdout == REDIR_OUT_APPEND) //11
            {
                flag |= O_APPEND;
            }
            else if (cmd->attr.redir_stdout == REIDR_OUT_CLEAR) //01
            {
                flag |= O_TRUNC;
            }
            else //10
            {
                fprintf(stderr, "In external handler: cmd attribution redir_stdout=10, failed\n");
                exit(EXIT_FAILURE);
            }
            int file = open(cmd->dst_file, flag, 0666);
            if (file == -1)
            {
                fprintf(stderr, "In external handler, %d line: open failed\n", __LINE__);
                exit(EXIT_FAILURE);
            }
            dup2(file, 1);
            close(file);
        }

        //<, <<
        //cat << a: means cat finish by token a
        if (cmd->attr.redir_stdin)
        {
            if (cmd->attr.redir_stdin != REDIR_IN1)
            {
                fprintf(stderr, "In external handler: cmd attrbution redir_stdin!=01, failed\n");
                exit(EXIT_FAILURE);
            }
            int file = open(cmd->src_file, O_CREAT | O_RDONLY);
            if (file == -1)
            {
                fprintf(stderr, "In external handler, %d line: open failed\n", __LINE__);
                exit(EXIT_FAILURE);
            }
            dup2(file, 0);
            close(file);
        }
        
        execvp(name, argv);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}
