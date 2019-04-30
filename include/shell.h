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

#include <sys/types.h>
#include <sys/stat.h>

#include <readline/readline.h>
#include <readline/history.h>

//functions declaration
void ui_mainloop();


#endif
