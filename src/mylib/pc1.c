#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define item_type char
#define BUFF_SIZE 4
#define ITEM_NUM 8
struct buffer
{
    item_type items[BUFF_SIZE];
    int in;
    int out;
};
struct buffer buff1, buff2;
pthread_mutex_t mutex1, mutex2;
pthread_cond_t buff1_not_full, buff2_not_full;
pthread_cond_t buff1_not_empty, buff2_not_empty;
//-----functions delc--------
int is_empty(struct buffer *b);
int is_full(struct buffer *b);
int get_item(struct buffer *b);
void put_item(struct buffer *b, item_type item);
void init();
void *produce(void *arg);
void *calculate(void *arg);
void *consume(void *arg);

int main()
{
    init();
    pthread_t pro_tid, cal_tid, con_tid;
    pthread_create(&pro_tid, NULL, produce, NULL);
    pthread_create(&cal_tid, NULL, calculate, NULL);
    pthread_create(&con_tid, NULL, consume, NULL);
    pthread_join(pro_tid, NULL);
    pthread_join(cal_tid, NULL);
    pthread_join(con_tid, NULL);
    return 0;
}

void init()
{
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_cond_init(&buff1_not_empty, NULL);
    pthread_cond_init(&buff1_not_full, NULL);
    pthread_cond_init(&buff2_not_empty, NULL);
    pthread_cond_init(&buff2_not_full, NULL);
}

void *produce(void *arg)
{
    int i;
    item_type item;
    for (i = 0; i < ITEM_NUM; i++)
    {
        pthread_mutex_lock(&mutex1);
        while (is_full(&buff1))
            pthread_cond_wait(&buff1_not_full, &mutex1);
        item = 'a' + i;
        put_item(&buff1, item);
        printf("producer put [%c] into buff1\n", item);
        pthread_cond_signal(&buff1_not_empty);
        pthread_mutex_unlock(&mutex1);
    }
    return NULL;
}
void *calculate(void *arg)
{
    int i;
    item_type item;
    for (i = 0; i < ITEM_NUM; i++)
    {
        pthread_mutex_lock(&mutex1);
        while (is_empty(&buff1))
            pthread_cond_wait(&buff1_not_empty, &mutex1);
        item = get_item(&buff1);
        printf("\tcalculator get [%c] from buff1\n", item);
        pthread_cond_signal(&buff1_not_full);
        pthread_mutex_unlock(&mutex1);

        pthread_mutex_lock(&mutex2);
        while (is_full(&buff2))
            pthread_cond_wait(&buff2_not_full, &mutex2);
        item = item - 'a' + 'A';
        printf("\tcalculator put [%c] into buff2\n", item);
        put_item(&buff2, item);
        pthread_cond_signal(&buff2_not_empty);
        pthread_mutex_unlock(&mutex2);
    }
    return NULL;
}
void *consume(void *arg)
{
    int i;
    item_type item;
    for (i = 0; i < ITEM_NUM; i++)
    {
        pthread_mutex_lock(&mutex2);
        while (is_empty(&buff2))
            pthread_cond_wait(&buff2_not_empty, &mutex2);
        item = get_item(&buff2);
        printf("consumer get [%c] from buff2\n", item);
        pthread_cond_signal(&buff2_not_full);
        pthread_mutex_unlock(&mutex2);
    }
    return NULL;
}

int is_empty(struct buffer *b)
{
    return b->in == b->out;
}
int is_full(struct buffer *b)
{
    return (b->in + 1) % BUFF_SIZE == b->out;
}
int get_item(struct buffer *b)
{
    item_type item = b->items[b->out];
    b->out = (b->out + 1) % BUFF_SIZE;
    return item;
}
void put_item(struct buffer *b, item_type item)
{
    b->items[b->in] = item;
    b->in = (b->in + 1) % BUFF_SIZE;
}
