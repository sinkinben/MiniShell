#ifndef SHELL_H
#define SHELL_H


#include "types.h"
#include "macro.h"
#include "mystring.h"
#include "process.h"
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>

//functions declaration
void ui_mainloop();
int make_cmds(char buf[], int buf_size, cmd_t cmds[], int cmds_size);
bool scan_dir(const char *path, const char *target);

#endif
