/* change.c */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include "header.h"
#define SLEEPLEN 15
#define DEBUG

int main(){
    int     i,id, student_id,id2;
    struct  StudentInfo* infoptr;
    int     sema_set;
    int*    read_count;
    char    arr_student_id[50], password[10], newAddress[50], newSSN[50], newTEL[50], newName[50];
    int     id_found;
    
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

    /* Prompt the user for password */
    printf("Enter password : ");
    my_fgets(password, 10, stdin);

    /* Validate the password */
    if( strcmp( password, "000") != 0){
        printf("Wrong Password\n");
        exit(0);
    }

    printf("Enter id of the student to modify : ");
    my_fgets( arr_student_id, 50, stdin);
    student_id = atoi(arr_student_id);

    printf("Enter new ID (or NA if not changing) : ");
    my_fgets( newSSN, 50, stdin);

    printf("Enter new name (or NA if not changing) : ");
    my_fgets( newName, 50, stdin);

    printf("Enter new address (or NA if not changing) : ");
    my_fgets( newAddress, 50, stdin);
    
    printf("Enter new telephone number (or NA if not changing) : ");
    my_fgets( newTEL, 50, stdin);


    Wait(sema_set, 0);
    
    #ifndef DEBUG
    printf("change.c entering critical section\n");
    #endif


    id_found = 0;

    /* Search the shared memory with provided id */
    for( i=0; i<LEN; ++i){
        if( strcmp(infoptr[i].SSN, arr_student_id) == 0 ){
            id_found = 1;
            break;
        }
    }

    if( id_found){
        if( strcmp( newSSN, "NA") != 0){
            strcpy( infoptr[i].SSN, newSSN);
        }
        
        if( strcmp( newName, "NA") != 0){
            strcpy( infoptr[i].name, newName);
        }
        
        if( strcmp( newAddress, "NA") != 0){
            strcpy( infoptr[i].address, newAddress);
        }
        
        if( strcmp( newTEL, "NA") != 0){
            strcpy( infoptr[i].telNumber, newTEL);
        }

    } 
    else {
        printf( "\nRecord not found.\n\n");
    }

    sleep(SLEEPLEN);

    #ifndef DEBUG
    printf("change.c leaving critical section\n");
    #endif
    Signal(sema_set, 0);

    return 0;
}
