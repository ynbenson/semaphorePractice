/*header.c*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "header.h"

/**
 * @brief   Lock the semaphore n of the semaphore set semaph 
 * @note
 *      when sem_op is negative, and |sem_op| > semaphore value,
 *          process will block until value of semaphore becomes
 *          greater than or equal to |sem_op|
 *      When sem_op is negative, and |sem_op| < semaphore value,
 *          |sem_op| will be subtracted from value of semaphore
 *
 */
void Wait(int semaph, int n){
    struct sembuf sop; /* ??? */
    sop.sem_num = n; // which semaphore to operate
    sop.sem_op = -1; // semaphore operation
    sop.sem_flg = 0;
    semop(semaph, &sop, 1);
}

/**
 * @brief  Unlock the semaphore n of the semaphore set semaph
 * @note
 *      when sem_op is greater than zero, value of semaphore
 *      increments by sem_op
 */
void Signal(int semaph, int n){
    struct sembuf sop;
    sop.sem_num = n;
    sop.sem_op = 1;
    sop.sem_flg = 0;
    semop(semaph, &sop, 1);
}

/**
 * @brief   Make an array of n semaphores with key k
 */
    // only used for Load to create semaphores 
int GetSemaphs(key_t k, int n){
    int semid, i;

    if( (semid = semget(k, n, IPC_CREAT|0666)) != -1){
        for(i=0; i<n; ++i){
            Signal(semid, i);
        }
    }
 
    if(semid < 0){
        perror("create: semget failed");
        exit(2);
    }

   return semid;
}

/**
 * @brief   wrapper function for shmget
 */
int my_shmget(key_t key, int segsize, int flag){
    int id = shmget(key, segsize, flag);
    if(id < 0){
        perror("create: shmget failed");
        exit(1);
    }
    return id;
}

/*
*
 * @brief   wrapper function for shmat
 *
void my_shmat(int id, void* cp, int flag){
    ptr = shmat(id, cp, flag);
    if(ptr <= (*ptr)(0)){
        perror("create: shmat failed");
        exit(2);
    }
}
*/


/*
 * @brief   wrapper function for fopen
 */
FILE* my_fopen(char* filename, char* option){
    FILE* fp;
    if( (fp = fopen(filename, option)) == NULL){
        perror("fopen");
        exit(1);
    }
    return fp;
}

void printStudentInfo(struct StudentInfo* ptr){
    printf("\n****** Printing out ID %s Information ******\n", ptr->SSN); 
    printf("%-9s : %s\n", "Name", ptr->name); 
    printf("%-9s : %s\n", "Address", ptr->address);
    printf("%-9s : %s\n\n", "TEL", ptr->telNumber);  
}

/*
 * @brief   wrapper function for fgets 
 */
void my_fgets(char* buf, int n, FILE* fp){
    fgets(buf, n, fp);
    buf[strlen(buf)-1] = '\0';
}
