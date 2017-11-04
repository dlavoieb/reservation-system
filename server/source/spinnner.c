//
// Created by david on 04/11/17.
//

#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include "spinner.h"

pthread_t tid;
volatile int kill_cursor_flag;

void advance_cursor()
{
    static int pos=0;
    char cursor[4]={'/','-','\\','|'};
    printf("\b%c", cursor[pos]);
    fflush(stdout);
    pos = (pos+1) % 4;
}

void *do_cursor(void* vargp)
{
    int *verbose = (int*)vargp;
    if (*verbose) {
        struct timeval current_time, last_time;
        gettimeofday(&current_time, NULL);
        gettimeofday(&last_time, NULL);
        while (kill_cursor_flag == 0) {
            gettimeofday(&current_time, NULL);
            long usec_comp = (current_time.tv_sec - last_time.tv_sec) > 0 ? current_time.tv_usec + 1000000
                                                                          : current_time.tv_usec;
            if ((usec_comp - last_time.tv_usec) > 80000) {
                last_time.tv_usec = current_time.tv_usec;
                last_time.tv_sec = current_time.tv_sec;
                advance_cursor();
            }
        }
    }
    pthread_exit(NULL);
}