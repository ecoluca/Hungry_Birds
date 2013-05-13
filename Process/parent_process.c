/* 
 * test_parent.c  -  Process Example Application
 *
 * Copyright (C) 2013 Luca Contri
 * Author: Luca Contri <LucaContri86@gmail.com>
 *
 *
 * Command Compile: 
 * $ gcc -Wall -o hangry_bird  hangry_bird_process.c
 *    
 *
 * Esercizio per verificare l'uso dei socket e delle Shared Memory
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netdb.h>




/* Shared Memory */
#include <sys/ipc.h> 
#include <sys/shm.h> 

/* Semaphore */
#include <semaphore.h>
#include <sys/sem.h>

#define SEM_ACCESS   1
#define SEM_START    1

#define FULL_PLATE   7
#define NUMBER_BIRD 10
#define NAME_BIRD   10
#define NUM_CHILD    3


int *shm_addr ;
int worms = FULL_PLATE;
sem_t sem  ;


struct SH_MEMORY {

    int worms ;

};




/* Declared Function */
void child_create (void) ;
void parent_create (void) ;



int main(int argc, char* argv[]){

    /* Declared Shared Memory Variables */
    struct SH_MEMORY *food ;
    key_t  shm_key;
    size_t  shm_size;
    int  shm_flg;

    int shm_id;

    /* Declared Semaphore Variable */
    int pshared = SEM_ACCESS;
    unsigned int value = SEM_START;

    
    shm_key = 1986;
    shm_size = sizeof(worms); //sizeof(*food);
    
    /* Initialize Shared Memory */
    if ((shm_id = shmget (shm_key, shm_size, IPC_CREAT | 0666)) == -1) {
        perror("shmget: shmget failed"); exit(1); } else {
        (void) fprintf(stderr, "shmget: shmget returned %d\n", shm_id);
    }
  
    /* Attach to Shared Memory */
    shm_addr = shmat(shm_id, (void *)0, 0);
    if(shm_addr == (int *)(-1))
        perror("shmat");
    
    /* Write initial value to shared memory */
    *shm_addr = worms; //FULL_PLATE;

    /* Initialize Semaphore */
    if((sem_init(&sem, pshared, value)) == 1)
        {
            perror("Error initializing semaphore");
            exit(1);
        }
    
   
    /* Parent Function */

    /* Write to Shared Memory */
    while(1) {
        sleep(1);
        printf("[PID %d ] I am DAD... On plate there are %d worms. I'm Waiting ...\n", getpid(), *shm_addr);
        
        if( *shm_addr == 0 ){
            sem_wait(&sem);
            *shm_addr = *shm_addr + FULL_PLATE ;
            sem_post(&sem);
            printf("[PID %d ] I am DAD!!!  Now there are %d worms on plates\n", getpid(), *shm_addr);
        }
    }
    
    return 0;

}

