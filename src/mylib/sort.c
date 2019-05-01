/*************************************************************************
	> File Name: sort.c
	> Author: sinkinben
	> E-mail: sinkinben@qq.com 
	> Created Time: Sat 20 Apr 2019 10:12:38 AM CST
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100000
#define NR_TOTAL SIZE
#define NR_CPU 2
#define NR_CHILD (NR_TOTAL / NR_CPU)
int array[SIZE];
int buff[SIZE];
typedef struct
{
	int *a;
	int start, end;
} param;
param params[NR_CPU];
pthread_t tid[NR_CPU];
int cmp(const void *a, const void *b)
{
	return (*(int *)a) - (*(int *)b);
}

void *sub_thread(void *arg)
{
	param *p = (param *)arg;
	qsort(p->a + p->start, p->end - p->start, sizeof(int), cmp);

	return NULL;
}
void merge(int a[]) //only support merge 2 parts by now
{
	int i = 0;
	int j = NR_CHILD;
	int k = 0;
	int endi = i + NR_CHILD;
	int endj = j + NR_CHILD;
	while (i < endi && j < endj)
	{
		if (a[i] < a[j])
			buff[k++] = a[i++];
		else
			buff[k++] = a[j++];
	}
	while (i < endi)
		buff[k++] = a[i++];
	while (j < endj)
		buff[k++] = a[j++];
	memcpy(array, buff, sizeof(buff));
}
int main()
{
	int i = 0;
	for (i = 0; i < SIZE; i++)
	{
		array[i] = SIZE - i;
	}

	for (i = 0; i < NR_CPU; i++)
	{
		param *p = &params[i];
		p->a = array;
		p->start = i * NR_CHILD;
		p->end = p->start + NR_CHILD;
		pthread_create(&tid[i], NULL, sub_thread, (void *)p);
	}

	for (i = 0; i < NR_CPU; i++)
	{
		pthread_join(tid[i], NULL);
	}
	merge(array);
	for (i = 0; i < 50; i++)
		printf("%5d ", array[i]);
	puts("");
}
