#ifndef TYPES_H
#define TYPES_H
#define true 1
#define false 0

typedef unsigned char bool;

#define REDIR_IN1 (0x1)
#define REDIR_IN2 (0x3)
#define REDIR_OUT_APPEND (0x3)
#define REIDR_OUT_CLEAR (0x1)
#define PIPE_IN (0x2)
#define PIPE_OUT (0x1)
#define WAIT_BKGD (0x1)
typedef union {
    struct
    {
        unsigned int redir_stdout : 2; //00 means no redirection, 01 means '>', 11 means '>>'
        unsigned int redir_stdin : 2;  //00 means no redirection, 01 means '<', 11 means '<<'
        unsigned int pipe_flag : 2;    //00 means no pipe, 10 means pipe-in-cmd, 01 means pipe-out-cmd, 11 means pipe-in&out
        unsigned int wait_flag : 1;    //1 means '&', 0 means nothing
    };
    unsigned int attr_val;
} cmd_attr;

#define CMD_NAME_LEN 32
#define CMD_ARGS_LEN 64
#define CMD_ARGV_LEN 32
#define CMD_FILE_LEN 16
typedef struct
{
    char name[CMD_NAME_LEN];
    char args[CMD_ARGS_LEN];
    char src_file[CMD_FILE_LEN]; //{<, <<} + src
    char dst_file[CMD_FILE_LEN]; //{>, >>} + dst
    cmd_attr attr;
} cmd_t;

#endif