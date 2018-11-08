/* load.c */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include "header.h"
#define N 50 
#define Num_StructData 4
#define DEBUG
#define SLEEP_LEN 5

int main(){
    int     i,id, id2;
    struct  StudentInfo* infoptr;
    int     sema_set;
    int*    read_count;
    int     numStruct;


    /* get shared memory to store data */
    id = my_shmget(KEY, SEGSIZE, IPC_CREAT | 0666);
    id2 = my_shmget(INTKEY, INTSIZE, IPC_CREAT | 0666);
    /* attach the shared memory segment to the process's addr space */
    infoptr = (struct StudentInfo*)shmat(id, 0, 0); 
    read_count = (int*)shmat(id2, 0, 0);
    // numStruct = shmat(id, 0, 0);

    if( infoptr <= (struct StudentInfo*)(0) || read_count <= (int*)(0)){
        perror("shmat");
        exit(1);
    }

    /* get a set of NUM_SEMAPHS semaphores */
    sema_set = GetSemaphs(SEMA_KEY, NUM_SEMAPHS);

    Wait(sema_set, 0);

    /* initialize shared memory "read_count" */
    (*read_count) = 0;

    /* store data in the shared memory segment */
    FILE* fp = NULL;
    char* filename = "sampleInput.txt";
    char line[N] = {'\0'};

    fp = my_fopen( filename, "r");

    struct StudentInfo* temp = infoptr;
    int infoType = 0;
    int numEntry = 0;

    while( fgets( line, N, fp) != NULL){
        //printf( "line : %s\n", line);
        line[strlen(line)-1] = '\0'; /* get rid of \n */
        switch(infoType){
            case 0: strcpy( temp->name, line); break;
            case 1: strcpy( temp->SSN, line); break;
            case 2: strcpy( temp->address, line); break;
            case 3: strcpy( temp->telNumber, line); temp++; break;
            default: printf("this shouldn't happen\n");
        }
        numEntry++;
        infoType = (infoType+1) % Num_StructData;
    }
    
    temp=NULL;

    if( fclose(fp) == EOF){
        perror("fclose");
        exit(1);
    }

//
//    (numStruct) = numEntry / Num_StructData; 
//    
//    #ifndef DEBUG
//    printf("numStruct: %d\n", (numStruct) ); 
//    #endif
// 
//    
    sleep(SLEEP_LEN);
/*    
    for(i=0; i<(numStruct); ++i){
        printf("Name : %s\tID : %s\tAddress : %s\tTEL : %s\n",
                infoptr[i].name, infoptr[i].SSN, infoptr[i].address, infoptr[i].telNumber);
    }
*/

    Signal(sema_set, 0); 

    exit(0); 
}


