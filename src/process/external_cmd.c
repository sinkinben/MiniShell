#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "process.h"
#include "macro.h"
void external_handler(cmd_t cmds[], int cmd_num)
{
    log("not finish external command\n");
    assert(0);
}