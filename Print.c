/* query.c */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include "header.h"
#define DEBUG
int main(){
    int     i,id,id2;
    struct  StudentInfo* infoptr;
    int     sema_set;
    int*    read_count;
    int     student_id;
 
    /* get shared memory to store data */
    id = my_shmget(KEY, SEGSIZE, IPC_CREAT | 0666);
    id2 = my_shmget(INTKEY, INTSIZE, IPC_CREAT | 0666);
   
    /* attach the shared memory segment to the process's addr space */
    infoptr = (struct StudentInfo*)shmat(id, 0, 0); 
    read_count = (int*)shmat(id2, 0, 0);
    
    if( infoptr <= (struct StudentInfo*)(0) || read_count <= (int*)(0)){
        perror("shmat");
        exit(1);
    }
    
    /* get a set of NUM_SEMAPHS semaphores */
    sema_set = semget(SEMA_KEY, 0, 0);
   
    Wait(sema_set, 1);
    

    (*read_count)++;

    if(*read_count == 1){
        Wait(sema_set, 0);
    }

    Signal(sema_set, 1);


    for(i=0; i<LEN && strcmp(infoptr[i].name,"") != 0; ++i){
        printStudentInfo( infoptr + i);
    }
    
    Wait(sema_set, 1);
    
    (*read_count)--;
    
    if( (*read_count) == 0){
        Signal(sema_set, 0);
    }

    #ifndef DEBUG
    printf("I am Print.c and I am leaving critical section\n");
    #endif
    
    Signal(sema_set, 1);

    /* detach the shared memory segment */
    shmdt( infoptr); 
    shmdt( read_count);


    return 0;
}
