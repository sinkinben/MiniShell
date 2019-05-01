#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define N 8
pthread_mutex_t mutex;
pthread_cond_t cond;
int current_order = 0;
void *thread_run(void *arg)
{
    const int order = (*((int *)arg));
    int i = 2;
    while (i--)
    {
        pthread_mutex_lock(&mutex);
        while (order != (current_order % N))
            pthread_cond_wait(&cond, &mutex);
        printf("thread[%d] get  number '%d'\n", order, current_order);
        //current_order = (current_order + 1) % N;
        current_order = (current_order + 1);
        printf("thread[%d] pass number '%d' to next one\n", order, current_order);
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    int order[N]; //给每个线程分配一个号码
    int i = 0;
    pthread_t tid[N];
    for (i = 0; i < N; i++)
    {
        order[i] = i;
        pthread_create(&tid[i], NULL, thread_run, &order[i]);
    }
    for (i = 0; i < N; i++)
    {
        pthread_join(tid[i], NULL);
    }
    return 0;
}
