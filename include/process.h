#ifndef PROCESS_H
#define PROCESS_H

int buildin_handler();
int buildin_cd(char *);
int buildin_pwd(char *);
int buildin_exit(char *);

extern struct passwd *user;

#endif