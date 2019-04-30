#ifndef PROCESS_H
#define PROCESS_H

int buildin_handler(cmd_t *);
int buildin_cd(char *);
int buildin_pwd(char *);
int buildin_exit(char *);

void external_handler();

extern struct passwd *user;

#endif