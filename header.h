/* header.h */
#define KEY ((key_t)(10001)) 
#define INTKEY ((key_t)(10002)) 
#define SEGSIZE sizeof(struct StudentInfo) * 20
//#define SEGSIZE sizeof(arr_StudentInfo)
#define NUM_SEMAPHS 2 
#define SEMA_KEY ((key_t)(1001))
#define LEN 20
#define INTSIZE sizeof(int)

struct StudentInfo{
    char name[50];
    char SSN[50];
    char address[50];
    char telNumber[50];
};

struct StudentInfo arr_StudentInfo[ LEN ]; 

void my_fgets(char* buf, int n, FILE* fp);  
void Wait(int semaph, int n);
void Signal(int semaph, int n);
int GetSemaphs(key_t k, int n);
FILE* my_fopen(char* filename, char* option);
int my_shmget(key_t key, int segsize, int flag);
void printStudentInfo(struct StudentInfo* ptr);
