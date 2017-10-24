//
// Created by david on 19/10/17.
//

#ifndef RESERVATION_SYSTEM_COMMON_TYPES_H
#define RESERVATION_SYSTEM_COMMON_TYPES_H

#define MAX_NUM_TABLES 20

#define NO_REPLY 0
#define ACCEPTED 1
#define DENIED 2

typedef struct {
    char name[100];
    char section;
    __uint8_t table;
} demand_t;

typedef struct {
    char name[100];
} reservation_t;

typedef struct  {
    int num_demands;
    demand_t demands[MAX_NUM_TABLES];
    int demand_results[MAX_NUM_TABLES];
    reservation_t reservations[MAX_NUM_TABLES];
}shr_memory_t;

#endif //RESERVATION_SYSTEM_COMMON_TYPES_H
