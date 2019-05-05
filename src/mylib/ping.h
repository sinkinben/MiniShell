#ifndef __PING_H__
#define __PING_H__

#include <stdio.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/ip_icmp.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>
#include <math.h>

#define ICMP_DATA_LEN 56 //ICMP默认数据长度
#define ICMP_HEAD_LEN 8  //ICMP默认头部长度
#define ICMP_LEN (ICMP_DATA_LEN + ICMP_HEAD_LEN)
#define SEND_BUFFER_SIZE 128 //发送缓冲区大小
#define RECV_BUFFER_SIZE 128 //接收缓冲区大小
#define SEND_NUM 10          //发送报文数
#define MAX_WAIT_TIME 3

#define DEBUG
#endif