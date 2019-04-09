#ifndef TYPES_H
#define TYPES_H
#define true 1
#define false 0

typedef unsigned char bool;

typedef union
{
    struct{
        unsigned int redir_stdout:2;    //00 means no redirection, 01 means '>' to files, 10 means '>>'
        unsigned int redir_stdin:2;     //00 means no redirection, 01 means '<' 
    };
    unsigned int attr_val;
}cmd_attr;


#define CMD_NAME_LEN 10
#define CMD_ARGV_LEN 64
typedef struct
{
    char name[CMD_NAME_LEN];
    char *argv[CMD_ARGV_LEN];
}cmd_t;

#endif