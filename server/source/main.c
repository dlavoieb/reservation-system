#include <stdio.h>
#include <common_types.h>
#include <names.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <memory.h>
#include <stdlib.h>
#include <signal.h>


void interrupt_handler(int signum)
{
    printf("\b\bExiting reservation server...\n");
    shm_unlink(SHARED_MEMORY_NAME);
    sem_unlink(MEM_SEM_NAME);
    printf("Done!\n");
    exit(EXIT_SUCCESS);
}

int main()
{

    /*
     * Plan:
     * Initialize shared memory
     * Create named semaphores
     * Post on semaphore
     * Enter wait loop
     */

    size_t mem_size = sizeof(shr_memory_t);
    signal(SIGINT, interrupt_handler);
    signal(SIGHUP, interrupt_handler);
    signal(SIGTERM, interrupt_handler);

    int fd = shm_open(SHARED_MEMORY_NAME, O_RDWR | O_CREAT, 0644);

    ftruncate(fd, mem_size);

    shr_memory_t *memory = (shr_memory_t *) mmap(NULL, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_t *dem_sem;
    dem_sem = sem_open(MEM_SEM_NAME, O_CREAT, 0644, 1);

//    char *temp_name = "David";
//    memory->demands[0].name = temp_name;
//    memory->demands[0].section = 'A';
//    memory->demands[0].table = 106;
//    memory->num_demands = 1;

    while (1) {
        sem_wait(dem_sem);
        if (memory->num_demands > 0) {
            // process demand
            for (int i = 0; i < MAX_NUM_TABLES; i++) {
                // find first demand to process
                if (memory->demands[i].name && memory->demand_results[i] == NO_REPLY) {
                    demand_t *current_demand = &(memory->demands[i]);
                    // demand has a non empty name and was not answered
                    if (current_demand->table != 0) {
                        // request for a specific table was made
                        int temp_section = current_demand->table / 100 - 1;
                        if (temp_section + 'A' != current_demand->section) {
                            // shit hit the fan, the table number is not in the right section
                            memory->demand_results[i] = DENIED;
                        }
                        else {
                            int temp_table = current_demand->table % 100;
                            if (temp_table > MAX_NUM_TABLES / 2) {
                                // shit hit the fan, the table number is too big
                                memory->demand_results[i] = DENIED;
                            }
                            else {
                                int index = temp_section * 10 + temp_table;
                                if (memory->reservations[index].name[0] != '\0') {
                                    // too late, this is already reserved
                                    memory->demand_results[i] = DENIED;
                                }
                                else {
                                    // congrats, your table is reserved!
                                    strcpy(memory->reservations[index].name, current_demand->name);
                                    memory->demand_results[i] = ACCEPTED;
                                }
                            }
                        }
                    }
                    else {
                        // request did not specify a table, place in first available
                        int start_index = (current_demand->section - 'A') * 10;
                        for (int j = start_index; j < start_index + 10; j++) {

                            if (memory->reservations[j].name[0] == '\0') {
                                strcpy(memory->reservations[j].name, current_demand->name);
                                memory->demand_results[i] = ACCEPTED;
                                break;
                            }
                        }
                    }
                    memory->num_demands--;
                    break;
                }
            }
        }
        else {
            printf("No request to process\n");
        }
        sem_post(dem_sem);
    }
}
