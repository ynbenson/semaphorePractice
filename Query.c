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
    char    arr_student_id[15];
    
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

    /* get the id of the student the user wants to query */
    printf("Enter id of the student to query: ");
    my_fgets( arr_student_id, 50, stdin);

    Wait(sema_set, 1);
    
    (*read_count)++;


    if(*read_count == 1){
        Wait(sema_set, 0);
    }

    Signal(sema_set, 1);

    /* do the reading */
        
    #ifndef DEBUG
    printf("I am reader=>%d and I am entering critical section\n", *read_count);
    #endif

    /* search the shared memory for the record of the student */
    int id_found = 0;
    
    for(i=0; i<LEN; ++i){
        if( strcmp(infoptr[i].SSN, arr_student_id) == 0){
            id_found = 1;
            break;
        }
    }
    
    if( id_found ){
        printStudentInfo( infoptr + i);
    }
    else {
        printf( "\nRecord not found.\n\n");
    }

    sleep(10);

    #ifndef DEBUG
    printf("I am reader=>%d and I am leaving critical section\n", *read_count);
    #endif
    Wait(sema_set, 1);
    (*read_count)--;

    if( (*read_count) == 0){
        Signal(sema_set, 0);
    }

    Signal(sema_set, 1);

    /* detach the shared memory segment */
    shmdt( infoptr); 
    shmdt( read_count);


    return 0;
}
