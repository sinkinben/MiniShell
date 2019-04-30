#include <stdio.h>
#include "shell.h"
int main()
{
    printf("\033[1;31;47mWelcome to MiniShell v1.0\033[0m\n");
    ui_mainloop();
}