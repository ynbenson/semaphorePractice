/* clean.c */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include "header.h"

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
    sema_set = GetSemaphs(SEMA_KEY, NUM_SEMAPHS);

    Wait(sema_set, 0);

    /* write the contents of the shared memory to file
     * in the format of the input file */
    FILE* fp;
    char* filename = "output.txt";
    fp = my_fopen( filename, "w");

    for( i=0; i<LEN && strcmp(infoptr[i].name,"") != 0; ++i){
        fprintf( fp, "%s\n", infoptr[i].name);
        fprintf( fp, "%s\n", infoptr[i].SSN);
        fprintf( fp, "%s\n", infoptr[i].address);
        fprintf( fp, "%s\n", infoptr[i].telNumber);
    }

    fclose(fp);
    
    /* detach the shared memory segment */
    shmdt( arr_StudentInfo); 
    shmdt( read_count); 
   
    /* destory the shared memory segment */
    shmctl( id, IPC_RMID, (struct shmid_ds*) 0);
    shmctl( id2, IPC_RMID, (struct shmid_ds*) 0);

    sleep(5);
    Signal(sema_set, 0);

    /* Remove the semaphore set */
    semctl( sema_set, 0, IPC_RMID); 
  
    return 0;
}

