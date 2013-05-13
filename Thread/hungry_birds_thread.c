/* 
 * hangry_bird_thread.c  -  Thread Example Application
 *
 * Copyright (C) 2013 Luca Contri
 * Author: Luca Contri <LucaContri86@gmail.com>
 *
 *
 * Command Compile: 
 * $ gcc -Wall -lpthread -o hangry_bird  hangry_bird_thread.c
 *    
 *
 * NOTE: THREAD's exercises 
 *       Use of Mutex, Thread e Condition Variables
 */


#ifndef _HANGRY_BIRD_THREAD_C_
#define _HANGRY_BIRD_THREAD_C_

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>  /* sleep */


#define FULL_PLATE   7
#define NUMBER_BIRD 10
#define NAME_BIRD   10


pthread_cond_t var_cond ;
pthread_mutex_t mutex_cond;
pthread_mutex_t sem;
int worms = FULL_PLATE;
int scream = 0 ;


struct DATA {
    int num ;
    char name[NAME_BIRD];
} ;


void *parent(void *dad){
    struct DATA *aa = (struct DATA *)dad ; 
    while(1){
        pthread_mutex_lock(&mutex_cond);
        if(pthread_cond_wait(&var_cond, &mutex_cond) == 0){
            //        pthread_cond_wait(&var_cond, &mutex_cond);
            /* Take Mutex */
            //pthread_mutex_lock(&sem); 
            worms = (rand()%FULL_PLATE)+1;
            printf("\nParent %s put %d worms on plate.\n", aa->name, worms);
            /* Release Mutex */
            //pthread_mutex_unlock(&sem); 
        }
        pthread_mutex_unlock(&mutex_cond); 
    } 
}

void *child_handler(void *data){
    struct DATA *aa = (struct DATA *)data ; 
    while(1){
        sleep(rand()%4);
        if(worms > 0){
            /* Take Mutex */
                pthread_mutex_lock(&mutex_cond);
                //pthread_mutex_lock(&sem); 
            worms-- ;
            if(worms == 0){
                //pthread_mutex_lock(&mutex_cond);
                pthread_cond_signal(&var_cond);
                //pthread_mutex_unlock(&mutex_cond); 
            }
            printf("Bird N:%d take a worm. Remain %d worms\n",aa->num, worms);
            /* Release Mutex */
                pthread_mutex_unlock(&mutex_cond); 
                //pthread_mutex_unlock(&sem); 
        }
    }
}


int main (int argc, char* argv[]){
    
    int i = 0;    

    /* Init Empty Mutex */
    pthread_mutex_init (&sem, NULL);        
    pthread_mutex_init (&mutex_cond, NULL);        

    /* Init Thread */
    pthread_t thread_parent;
    pthread_t  thread_bird[NUMBER_BIRD];
                
    struct DATA data[NUMBER_BIRD];

    /* Control State */ 
    if(scream == 0 && worms == 0){
        scream = 1 ;
    }
    
    if (pthread_cond_init(&var_cond, NULL) < 0)
        { fprintf (stderr, "variabile condition error.\n");
            exit (1);
        } 

    /* Parent */
    struct DATA dad = {0, "BaBBo"} ;
    if (pthread_create(&thread_parent, NULL, parent, (void *)&dad ) < 0)
        { fprintf (stderr, "create error for PARENT.\n");
            exit (1);
        } 
    
    /* Child */
    for ( i = 0 ; i < NUMBER_BIRD ; i++){
        data[i].num = i ;
        if (pthread_create(&thread_bird[i], NULL, child_handler, (void *)&data[i] ) < 0)
            { fprintf (stderr, "create error for BIRD N%d.\n", data[i].num);
                exit (1);
            }
    }
    
    /* Creation JOIN */
    pthread_join (thread_parent, NULL);
    
    for ( i = 0 ; i < NUMBER_BIRD ; i++)
        { pthread_join (thread_bird[i], NULL);
        }
    
    return 0 ;
}

#endif /* _HANGRY_BIRD_THREAD_C_ */
