/** 
 * hangry_bird_POSIX_process.c  -  Process Example Application
 *
 * Copyright (C) 2013 Luca Contri
 * Author: Luca Contri <LucaContri86@gmail.com>
 *
 *
 * Command Compile: 
 * $ gcc -Wall -o hangry_bird  hangry_bird_POSIX_process.c
 *    
 *
 * Esercizio per verificare l'uso dei socket e delle Shared Memory
 *
 **/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/mman.h>
#include <sys/sem.h>
#include <arpa/inet.h>


#define SEM_ACCESS   1
#define SEM_START    1

#define FULL_PLATE   7
#define NUMBER_BIRD 10
#define NAME_BIRD   10
#define NUM_CHILD    2


#define NAME_SEM          "SEM_MUTEX"
#define NAME_SEM_PARENT   "SEM_PARENT"
#define NAME_SEM_BIRD     "SEM_BIRD"
#define NAME_SHM          "MY_SHARED_MEMORY"
#define PERMS             0666
#define FLAGS             O_CREAT | O_RDWR


struct SH_MEMORY {

    int worms ;

};

/* Global Variable */
int *shm_addr ;
int worms = FULL_PLATE;
sem_t *sem  ;
sem_t *sem_parent  ;
sem_t *sem_bird;
int shm_id;


/* Funcition */
void 
parent_create(void)
{
    printf("[PID %d ] I am DAD\n", getpid());
    /* Write to Shared Memory */
    while(1) {
        sem_wait(sem_parent);
         sem_wait(sem);
         if(*shm_addr == 0) {
            //sem_wait(sem);
            *shm_addr = *shm_addr + FULL_PLATE ;
            printf("[PID %d ] I am DAD!!!  Now there are %d worms on plates\n", getpid(), *shm_addr);
            // sem_post(sem);
            //sem_post(sem_parent);
             } else if(*shm_addr > 0){
            printf("[PID %d ] I am DAD... On plate there are %d worms. "
                   "I'm Waiting ...\n", getpid(), *shm_addr);
            } else { 
            perror("Acces Plate: ");
            exit(EXIT_FAILURE);
         }
         sem_post(sem);
         sem_post(sem_bird);    
    }
    perror("Parent");
    exit(EXIT_FAILURE);
}

void 
child_create (void) 
{
    printf("[PID %d ] I am Child\n", getpid());
    while(1) {
        sleep(rand()%5);
        sem_wait(sem_bird);
        sem_wait(sem);
        if(*shm_addr > 0) {
            worms = *shm_addr;
            *shm_addr = *shm_addr - 1;
            printf("Worms on plate:%d ... I Bird %d. I'm hangry ... "
                   "Remain %d Worms\n",worms, getpid(), *shm_addr);
            sem_post(sem);
            sem_post(sem_bird);
        } else if(*shm_addr == 0){
            printf("EHI DAD !!! The plate in Empty... We are hangry yet!!! \n");
            sem_post(sem);
            sem_post(sem_parent);
        } else {
            perror("Child");
            //sem_post(sem);
            exit(EXIT_FAILURE) ; 
        }
    } 
}

static void 
sigchld_handler(int signal)
{
	pid_t pid;
	int status;
    
    /* handle exit of more than one child */
	do {
		pid = waitpid(-1, &status, WNOHANG);
	} while (pid > 0);
}

static void 
sigterm_handler(int signal)
{
	/* Delete Shared Memory */
    if( shm_unlink(NAME_SHM) == -1 )
        exit(EXIT_FAILURE);
    if(sem_unlink(NAME_SEM_PARENT))
        exit(EXIT_FAILURE);
    if(sem_unlink(NAME_SEM))
        exit(EXIT_FAILURE);
    if(sem_unlink(NAME_SEM_BIRD))
        exit(EXIT_FAILURE);
    exit(EXIT_SUCCESS);
}



int main(int argc, char* argv[])
{
    /* Declared Shared Memory Variables */
    key_t  shm_key;
    size_t  shm_size;
    
    /* Declared Semaphore Variable */
    int pshared = SEM_ACCESS;
    unsigned int value = SEM_START;

    printf("[%d]\n", SEM_ACCESS);
    fflush(stdout);
    
    shm_key = 1986;
    shm_size = sizeof(worms); /* sizeof(*food); */
    
    /* Initialize Shared Memory */
    shm_id = shm_open( NAME_SHM, FLAGS, PERMS ) ;
    if (shm_id == -1)
        perror("shm_open: ");
    if (ftruncate(shm_id, shm_size) == -1)
        perror("ftruncate: ");
    
    /* Map shared memory object */
    shm_addr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);
    if (shm_addr == MAP_FAILED)
        perror("mmap: ");
    
    /* Write initial value to shared memory */
    *shm_addr = worms; //FULL_PLATE;
    
    /* Initialize Semaphore */
     if((sem = sem_open(NAME_SEM, O_CREAT, O_RDWR, 1)) == SEM_FAILED){
         perror("Error initializing semaphore");
        exit(1);
    }

     if((sem_bird = sem_open(NAME_SEM_BIRD, O_CREAT, O_RDWR, 1)) == SEM_FAILED){
         perror("Error initializing semaphore");
         exit(1);
     }
     
    /* Comunication Semaphore */
    if(*shm_addr == 0){
        if((sem_parent = sem_open(NAME_SEM_PARENT, O_CREAT, O_RDWR, 1)) == SEM_FAILED){
            perror("Error initializing PARENT semaphore");
            exit(1);
        }
    } else if(*shm_addr > 0) {
        if((sem_parent = sem_open(NAME_SEM_PARENT, O_CREAT, O_RDWR, 0)) == SEM_FAILED){
            perror("Error initializing PARENT semaphore");
            exit(1);
        }
    } else {
        perror("Error initializing PARENT semaphore: BAD value Shared Memory");
        exit(1);
    }
    
	/* register process signal handlers */
	signal(SIGINT, sigterm_handler);
	signal(SIGTERM, sigterm_handler);
	signal(SIGCHLD, sigchld_handler);
    
    /* Process Creation */
    int i;
    for(i = 0; i < NUM_CHILD; i++ ) { 
        switch (fork()) {
            case -1 :  // error occured
                perror("Fork Failed");
                exit(1);
                break;  
                
            case 0 : // child process
                signal(SIGINT, SIG_DFL);
                signal(SIGTERM, SIG_DFL);
                signal(SIGCHLD, SIG_DFL);
                child_create();
                exit(0);
                
            default : // parent process
                break;
        }
    }
    
    /* Parent operation */
    parent_create() ;
    
    return 0;   
}

