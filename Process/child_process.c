/* 
 * test_child.c  -  Process Example Application
 *
 * Copyright (C) 2013 Luca Contri
 * Author: Luca Contri <LucaContri86@gmail.com>
 *
 *
 * Command Compile: 
 * $ gcc -Wall -o FIGLIO  test_child.c
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
int worms ;
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
    if ((shm_id = shmget (shm_key, shm_size, 0666 )) == -1) {
        perror("shmget: shmget failed"); exit(1); } else {
        (void) fprintf(stderr, "shmget: shmget returned %d\n", shm_id);
    }
  
    /* Attach to Shared Memory */
    shm_addr = shmat(shm_id, (void *)0, 0);
    if(shm_addr == (int *)(-1))
        perror("shmat");
    
    /* Write initial value to shared memory */
    //*shm_addr = worms; 
    //FULL_PLATE;

    /* Initialize Semaphore */
    if((sem_init(&sem, pshared, value)) == 1)
        {
            perror("Error initializing semaphore");
            exit(1);
        }
    

    /* Child Function */

    while(1)
        {
            sleep(1);
            printf("[PID %d ] I am Child\n", getpid());
            if( *shm_addr > 0 ) {
                sem_wait(&sem);
                *shm_addr = *shm_addr - 1;
                if(*shm_addr != worms)
                    {
                        worms = *shm_addr;
                        printf("Worms on plate:%d ... I Bird %d. I'm hangry ... "
                               "Remain %d Worms\n",worms, getpid(), *shm_addr);
                    }
                sem_post(&sem);
            } if ( *shm_addr == 0 ){
                    /* Signal Send */
                    printf("%d worms... The plate ie EMPTY... I am hangry... DAD !!!\n", *shm_addr);
                } else {
                /* Error */
            }
        } 
    
    return 0;   
}






