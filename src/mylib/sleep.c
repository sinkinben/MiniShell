#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
 * For testing '&' token
 */
int main()
{
    int sec = 5;
    sleep(sec);
    printf("I have slept %d second(s).\n", sec);
    return 0;
}