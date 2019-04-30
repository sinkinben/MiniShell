#ifndef PROCESS_H
#define PROCESS_H
#include "types.h"
int buildin_handler(cmd_t *);
int buildin_cd(char *);
int buildin_pwd(char *);
int buildin_exit(char *);
int buildin_tree(char *args);
int buildin_help(char *args);
void tree(const char *dir, int depth);

void external_handler();

extern struct passwd *user;

#endif