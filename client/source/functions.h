//
// Created by david on 24/10/17.
//

#include <semaphore.h>
#include <common_types.h>

#ifndef RESERVATION_SYSTEM_FUNCTIONS_H
#define RESERVATION_SYSTEM_FUNCTIONS_H

void reserve_table(char *args[], int count, sem_t *dem_sem, sem_t *res_sem, shr_memory_t *memory);
void init_memory(sem_t *dem_sem, shr_memory_t *memory);
void print_status(sem_t *dem_sem, shr_memory_t *memory);
void clean_exit();
int getLine(char *prompt, char *args[]);

#endif //RESERVATION_SYSTEM_FUNCTIONS_H
