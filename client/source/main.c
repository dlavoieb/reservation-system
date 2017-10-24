#include <stdio.h>
#include <common_types.h>
#include <names.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <memory.h>
#include <stdlib.h>
#include <errno.h>
#include "functions.h"




int main()
{

    size_t mem_size = sizeof(shr_memory_t);
    int fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0644);
    if (fd == -1) {
        perror("shm_open() failed");
        exit(errno);
    }

    shr_memory_t *memory = (shr_memory_t *) mmap(NULL, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_t *dem_sem;
    dem_sem = sem_open(MEM_SEM_NAME, 0);
    sem_t *res_sem;
    res_sem = sem_open(RES_SEM_NAME, 0);

    int length;
    char *args[20];


    while(1)
    {
        length = getLine(">> ", args);

        if (length >= 1)
        {
            if (strcmp("reserve", args[0]) == 0) reserve_table(args, length, dem_sem, res_sem, memory); // exit command quits the shell

            else if (strcmp("init", args[0]) == 0) init_memory(dem_sem, memory);

            else if (strcmp("status", args[0]) == 0) print_status(dem_sem, memory);

            else if (strcmp("exit", args[0]) == 0) clean_exit();

            else
            {

            }
        }
    }
}
