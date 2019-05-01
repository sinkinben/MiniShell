/*************************************************************************
	> File Name: pi2.c
	> Author: sinkinben
	> E-mail: sinkinben@qq.com 
	> Created Time: Sat 20 Apr 2019 09:57:06 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define ITEM_MAX 500000
#define NR_TOTAL ITEM_MAX
#define NR_CPU 4
#define NR_CHILD (NR_TOTAL) / (NR_CPU)

typedef struct
{
    int start, end;
    double val;
} param;

typedef struct
{
    double val;
} result;
void *sub(void *arg)
{
    result *res;
    param *par;
    double val = 0;
    int i = 0;

    par = (param *)arg;
    for (i = par->start; i < par->end; i++)
    {
        if (i % 2 == 0)
            val += (double)1 / (2 * i + 1);
        else
            val -= (double)1 / (2 * i + 1);
    }
    res = (result *)malloc(sizeof(result));
    res->val = val;
    return res;
}
int main()
{
    pthread_t tid[NR_CPU];
    param params[NR_CPU];
    int i;

    for (i = 0; i < NR_CPU; i++)
    {
        param *p = &params[i];
        p->start = i * NR_CHILD;
        p->end = p->start + NR_CHILD;
        pthread_create(&tid[i], NULL, sub, p);
    }

    double pi = 0;
    for (i = 0; i < NR_CPU; i++)
    {
        result *r;
        pthread_join(tid[i], (void **)&r);
        pi += r->val;
        free(r);
    }
    printf("PI = %lf\n", pi * 4);
}
