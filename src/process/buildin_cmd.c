#include "shell.h"
struct
{
    char *name;
    int (*handler)(char *);
    char *doc;
} buildin_list[] = {
    {"cd", buildin_cd, "change current working directory"},
    {"pwd", buildin_pwd, "display current working directory"},
    {"exit", buildin_exit, "exit MiniShell"}};
#define NR_BUILDIN_LIST ((sizeof(buildin_list)) / (sizeof(buildin_list[0])))

int buildin_handler(char *argv[])
{
    int i = 0;
    for (i = 0; i < NR_BUILDIN_LIST; i++)
    {
        if (strcmp(buildin_list[i].name, argv[0]) == 0)
        {
            return buildin_list[i].handler(argv[1]);
        }
    }
    return -1;
}

int buildin_cd(char *args)
{
    if (args == NULL || strlen(args) == 0 || strcmp(args, "~") == 0)
    {
        args = user->pw_dir;
    }
    if (chdir(args) == -1)
    {
        warning("cd: dir '%s' not found\n", args);
        return -1;
    }
    return 0;
}

int buildin_pwd(char *args)
{
    char *p = NULL;
    if ((p = getcwd(NULL, 256)) == NULL)
        warning("pwd: error\n");
    puts(p);
    free(p);
    return 0;
}

int buildin_exit(char *args)
{
    exit(0);
    return 0;
}
