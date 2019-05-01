/*************************************************************************
	> File Name: pi1.c
	> Author: sinkinben
	> E-mail: sinkinben@qq.com 
	> Created Time: Sat 20 Apr 2019 09:44:19 AM CST
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#define ITEM_MAX 500000
double pi = 0;
double sub_pi = 0;
void *main_thread()
{
    int i = 0;
    for (i = 0; i < ITEM_MAX / 2; i++)
    {
        if (i % 2)
        {
            pi -= (double)(1) / (2 * i + 1);
        }
        else
        {
            pi += (double)(1) / (2 * i + 1);
        }
    }
}
void *sub_thread(void *arg)
{
    int i = 0;
    for (i = ITEM_MAX / 2 + 1; i < ITEM_MAX; i++)
    {
        if (i % 2)
        {
            sub_pi -= (double)1 / (2 * i + 1);
        }
        else
        {
            sub_pi += (double)1 / (2 * i + 1);
        }
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, sub_thread, NULL);
    main_thread();
    pthread_join(tid, NULL);
    pi += sub_pi;
    printf("PI = %.8lf\n", pi * 4);
}
