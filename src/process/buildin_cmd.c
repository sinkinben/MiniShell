#include "shell.h"
struct
{
    char *name;
    int (*handler)(char *);
    char *doc;
} buildin_list[] = {
    {"cd", buildin_cd, "change current working directory"},
    {"pwd", buildin_pwd, "display current working directory"},
    {"exit", buildin_exit, "exit MiniShell"},
    {"tree", buildin_tree, "list the dir structure"},
    {"help", buildin_help, "show help doc for build-in command"}};
#define NR_BUILDIN_LIST ((sizeof(buildin_list)) / (sizeof(buildin_list[0])))

int buildin_handler(cmd_t *cmd)
{
    int i = 0;
    for (i = 0; i < NR_BUILDIN_LIST; i++)
    {
        if (strcmp(buildin_list[i].name, cmd->name) == 0)
        {
            return buildin_list[i].handler(cmd->args);
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
void tree(const char *dir, int depth)
{
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
            printf("%*s%s/\n", depth, "", entry->d_name);
            tree(entry->d_name, depth + 4);
        }
        else
        {
            printf("%*s%s\n", depth, "", entry->d_name);
        }
    }
    chdir(cwd_buff); //switch to original dir
    closedir(dp);
}

int buildin_tree(char *args)
{
    if (args == NULL || strcmp(args, "") == 0)
        tree(".", 0);
    else
        tree(args, 0);
    return 0;
}

int buildin_help(char *args)
{
    int i;
    for (i = 0; i < NR_BUILDIN_LIST; i++)
    {
        printf("[%s]: %s\n", buildin_list[i].name, buildin_list[i].doc);
    }
}
