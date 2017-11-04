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
#include <pthread.h>
#include <argp.h>
#include "spinner.h"

const char *argp_program_version = "Reservation Server 1.0";
const char *argp_program_bug_address = "<david.lavoie-boutin@mail.mcgill.ca>";

struct arguments
{
    int verbose;              /* The -v flag */
} arguments;

static struct argp_option options[] =
{
    {"verbose", 'v', 0, 0, "Suppress the verbose output"},
    {0}
};

static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key)
    {
        case 'v':
            arguments->verbose = 0;
            break;
        case ARGP_KEY_END:
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static char doc[] = "Reservation Server -- A program to demonstrate inter-process communication for ECSE 427.";
static struct argp argp = {options, parse_opt, NULL, doc};

extern pthread_t tid;
extern int kill_cursor_flag;

void interrupt_handler(int signum)
{
    if (arguments.verbose) {
        printf("\n");
        switch (signum) {
            case SIGINT:
                printf("SIGINT received\n");
                break;
            case SIGHUP:
                printf("SIGHUP received\n");
                break;
            case SIGTERM:
                printf("SIGTERM received\n");
                break;
            case SIGSEGV:
                printf("SIGSEGV received\n");
                break;
            default:
                break;
        }
    }

    kill_cursor_flag=1;
    pthread_join(tid, NULL);
    if (arguments.verbose) printf("Exiting reservation server...\n");
    shm_unlink(SHARED_MEMORY_NAME);
    sem_unlink(MEM_SEM_NAME);
    sem_unlink(RES_SEM_NAME);
    if (arguments.verbose) printf("Done!\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv)
{
    arguments.verbose = 1;
    argp_parse (&argp, argc, argv, 0, 0, &arguments);

    size_t mem_size = sizeof(shr_memory_t);
    signal(SIGINT, interrupt_handler);
    signal(SIGHUP, interrupt_handler);
    signal(SIGTERM, interrupt_handler);
    signal(SIGSEGV, interrupt_handler);

    int fd = shm_open(SHARED_MEMORY_NAME, O_RDWR | O_CREAT, 0644);

    ftruncate(fd, mem_size);

    shr_memory_t *memory = (shr_memory_t *) mmap(NULL, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_t *dem_sem;
    dem_sem = sem_open(MEM_SEM_NAME, O_CREAT, 0644, 1);
    sem_t *res_sem;
    res_sem = sem_open(RES_SEM_NAME, O_CREAT, 0644, 0);

    pthread_create(&tid, NULL, do_cursor, (void*) &arguments.verbose);

    while (1) {
        sem_wait(dem_sem);
        if (memory->num_demands > 0) {
            // process demand
            for (int i = 0; i < MAX_NUM_TABLES; i++) {
                // find first demand to process
                if (memory->demands[i].name[0] != 0 && memory->demand_results[i] == NO_REPLY) {
                    demand_t *current_demand = &(memory->demands[i]);
                    // demand has a non empty name and was not answered
                    if (current_demand->table != 0) {
                        // request for a specific table was made
                        int temp_section = current_demand->table / 100 - 1;
                        if (temp_section + 'A' != current_demand->section) {
                            // shit hit the fan, the table number is not in the right section
                            memory->demand_results[i] = DENIED;
                            sem_post(res_sem);
                        }
                        else {
                            int temp_table = current_demand->table % 100;
                            if (temp_table > MAX_NUM_TABLES / 2) {
                                // shit hit the fan, the table number is too big
                                memory->demand_results[i] = DENIED;
                                sem_post(res_sem);
                            }
                            else {
                                int index = temp_section * 10 + temp_table;
                                if (memory->reservations[index].name[0] != '\0') {
                                    // too late, this is already reserved
                                    memory->demand_results[i] = DENIED;
                                    sem_post(res_sem);
                                }
                                else {
                                    // congrats, your table is reserved!
                                    strcpy(memory->reservations[index].name, current_demand->name);
                                    memory->demand_results[i] = ACCEPTED;
                                    sem_post(res_sem);
                                }
                            }
                        }
                    }
                    else {
                        // request did not specify a table, place in first available
                        int start_index = (current_demand->section - 'A') * 10;
                        int j;
                        for (j = start_index; j < start_index + 10; j++) {

                            if (memory->reservations[j].name[0] == '\0') {
                                strcpy(memory->reservations[j].name, current_demand->name);
                                memory->demand_results[i] = ACCEPTED;
                                sem_post(res_sem);
                                break;
                            }
                        }
                        if (j == start_index+10)
                        {
                            // did not find a free table in the section
                            memory->demand_results[i] = DENIED;
                            sem_post(res_sem);
                        }
                    }
                    memory->num_demands--;
                    break;
                }
            }
        }
        sem_post(dem_sem);
    }
}
