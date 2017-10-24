//
// Created by david on 24/10/17.
//

#include "functions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void reserve_table(char *args[], int count, sem_t *dem_sem, sem_t *res_sem, shr_memory_t *memory)
{
    if (count >= 3) {
        sem_wait(dem_sem);

        //find available reservation slot
        int slot_idx;
        for (slot_idx = 0; slot_idx < MAX_NUM_TABLES; slot_idx++) {
            if (memory->demands[slot_idx].name[0] == 0) {
                strcpy(memory->demands[slot_idx].name, args[1]);
                memory->demands[slot_idx].section = args[2][0];
                if (count >=4) memory->demands[slot_idx].table = (__uint8_t) atoi(args[3]);
                memory->demand_results[slot_idx] = NO_REPLY;
                memory->num_demands ++;
                break;
            }
        }
        printf("Placing reservation for %s\n", args[1]);
        sem_post(dem_sem);

        int result;
//        do
//        {
            sem_wait(res_sem);
            result = memory->demand_results[slot_idx];
//            if (result == NO_REPLY) sem_post(res_sem);
//        }
//        while(  result == NO_REPLY) ;


        char *reply;
        switch (result) {
            case NO_REPLY:
                reply = "No Reply";
                break;
            case ACCEPTED:
                reply = "Accepted";
                break;
            case DENIED:
                reply = "Denied";
                break;
        }
        printf("The demand for %s was %s\n",args[1], reply);

        sem_wait(dem_sem);
        strcpy(memory->demands[slot_idx].name, "");
        memory->demands[slot_idx].section = 0;
        memory->demands[slot_idx].table = 0;
        memory->demand_results[slot_idx] = NO_REPLY;
        sem_post(dem_sem);
    }
}

void init_memory(sem_t *dem_sem, shr_memory_t *memory)
{
    sem_wait(dem_sem);
    for (int i = 0; i<MAX_NUM_TABLES; i++) {
        strcpy(memory->reservations[i].name, "");
    }
    sem_post(dem_sem);

}

void print_status(sem_t *dem_sem, shr_memory_t *memory)
{
    sem_wait(dem_sem);
    printf("-------------------------\n");
    printf("| Section | Table | Name \n");
    printf("-------------------------\n");
    for (int i = 0; i<MAX_NUM_TABLES; i++) {
        printf("| %c       | %d   | %s\n", i/10 + 'A', ((i/10)+1)*100+i%10, memory->reservations[i].name);
    }
    printf("-------------------------\n");
    sem_post(dem_sem);
}

void clean_exit(){
    exit(0);
}

int getLine(char *prompt, char *args[])
{
    int i = 0;
    __ssize_t length;
    char *token, *loc;
    char *line = NULL;
    size_t linecap = 0;
    printf("%s", prompt);
    length = getline(&line, &linecap, stdin);
    if (length <= 0) {
        exit(-1);
    }

    while ((token = strsep(&line, " \t\n")) != NULL) {
        int j;
        for (j = 0; j < strlen(token); j++)
            if (token[j] <= 32)
                token[j] = '\0';
        if (strlen(token) > 0)
            args[i++] = token;
    }

    free(line);
    return i;
}

