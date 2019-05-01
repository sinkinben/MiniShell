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
struct sema_t
{
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};
typedef struct sema_t sema_t;
struct buffer buff1, buff2;
struct sema_t sema_mutex1, sema_mutex2;
struct sema_t sema_buff1_not_full, sema_buff1_not_empty;
struct sema_t sema_buff2_not_full, sema_buff2_not_empty;

//-----functions delc--------
int is_empty(struct buffer *b);
int is_full(struct buffer *b);
int get_item(struct buffer *b);
void put_item(struct buffer *b, item_type item);
void *produce(void *arg);
void *calculate(void *arg);
void *consume(void *arg);
void sema_init(sema_t *sema, int val);
void sema_wait(sema_t *sema);
void sema_signal(sema_t *sema);

int main()
{
    //init all
    sema_init(&sema_mutex1, 1);
    sema_init(&sema_mutex2, 1);
    sema_init(&sema_buff1_not_full, BUFF_SIZE - 1);
    sema_init(&sema_buff1_not_empty, 0);
    sema_init(&sema_buff2_not_full, BUFF_SIZE - 1);
    sema_init(&sema_buff2_not_empty, 0);

    pthread_t pro_tid, cal_tid, con_tid;
    pthread_create(&pro_tid, NULL, produce, NULL);
    pthread_create(&cal_tid, NULL, calculate, NULL);
    pthread_create(&con_tid, NULL, consume, NULL);
    pthread_join(pro_tid, NULL);
    pthread_join(cal_tid, NULL);
    pthread_join(con_tid, NULL);
    return 0;
}
void print_buff()
{
    printf("buff1: ");
    int i;
    for (i = buff1.in; i != buff1.out; i = (i + 1) % BUFF_SIZE)
    {
        printf("%c ", buff1.items[i]);
    }
    printf("\nbuff2: ");
    for (i = buff2.in; i != buff2.out; i = (i + 1) % BUFF_SIZE)
    {
        printf("%c ", buff2.items[i]);
    }
    puts("");
}
void sema_init(sema_t *sema, int val)
{
    sema->value = val;
    pthread_mutex_init(&sema->mutex, NULL);
    pthread_cond_init(&sema->cond, NULL);
}
void sema_wait(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    while (sema->value <= 0)
        pthread_cond_wait(&sema->cond, &sema->mutex);
    sema->value--;
    pthread_mutex_unlock(&sema->mutex);
}
void sema_signal(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    sema->value++;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
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
void *produce(void *arg)
{
    int i;
    item_type item;
    for (i = 0; i < ITEM_NUM; i++)
    {
        sema_wait(&sema_buff1_not_full);
        sema_wait(&sema_mutex1);
        item = 'a' + i;
        put_item(&buff1, item);
        printf("producer put [%c] into buff1\n", item);
        sema_signal(&sema_mutex1);
        sema_signal(&sema_buff1_not_empty);
    }
}
void *calculate(void *arg)
{
    int i;
    item_type item;
    for (i = 0; i < ITEM_NUM; i++)
    {
        sema_wait(&sema_buff1_not_empty);
        sema_wait(&sema_mutex1);
        item = get_item(&buff1);
        printf("\tcalculator get [%c] from buff1\n", item);
        sema_signal(&sema_mutex1);
        sema_signal(&sema_buff1_not_full);

        sema_wait(&sema_buff2_not_full);
        sema_wait(&sema_mutex2);
        item = item - 'a' + 'A';
        put_item(&buff2, item);
        printf("\tcalculator put [%c] into buff2\n", item);
        sema_signal(&sema_mutex2);
        sema_signal(&sema_buff2_not_empty);
    }
}
void *consume(void *arg)
{
    int i;
    item_type item;
    for (i = 0; i < ITEM_NUM; i++)
    {
        sema_wait(&sema_buff2_not_empty);
        sema_wait(&sema_mutex2);
        item = get_item(&buff2);
        printf("consumer get [%c] from buff2\n", item);
        sema_signal(&sema_mutex2);
        sema_signal(&sema_buff2_not_full);
    }
}