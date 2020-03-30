/* 

    >>  make
    >>  ./thread
    >> cat results.out

    CPU details:
        model name.............. Intel(R) Core(TM) i7-8565U CPU @ 1.80GHz
        CPU(s).................. 8
        On-line CPU(s) list..... 0-7
        Thread(s) per core...... 2
        Core(s) per socket...... 4
        L1d_cache_size.......... 32768 B
        L1d_assoc............... 8
        L1d_line_size........... 64 B
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TRIES 5                     //  number of tries a particular attacker thread performs
#define REPETITIONS 1000000         //  number of times a line from the cache is entirely filled
#define SIZE32KB (32*1024)          //  represents 32 KB
#define SIZE64KB (64*1024)          //  represents 64 KB
#define W 8                         //  associativity number of L1
#define STRIDE (SIZE32KB/W)         //  step distance between the consecutive accesses in order to fill a particular line of L1
#define VICTIM_START 1024           //  victim's first position to access in the array [0-4KB]
#define ATTACKER_DISTANCE 64        //  bytes space between each attacker thread [block size=64]
#define LOGFILE "results.out"       //  file to print the time results

char V[SIZE64KB];                   //  array to be accessed
pthread_mutex_t mutex 
    = PTHREAD_MUTEX_INITIALIZER;

typedef struct {                    //  struct to pass to thread function the input arguments and receive output
    double result;
    int index_min;
    int index_max;
    char * array;
    FILE * logfile;} atk_args;


double average(double * list);
void * create_victim(void * arg);
void * create_attacker(void * arg);


int main(void) {

    pthread_t atk, vict;
    atk_args *atk_args = malloc( sizeof(atk_args));
    FILE* logfile = fopen(LOGFILE,"w"); 

    
    if (pthread_create( &vict, NULL, create_victim, NULL) != 0)
        printf("\n__Unable to create Victim's thread");


    for ( int end = SIZE32KB; end <= SIZE64KB; end = end*2 ) {
        for (int start = end - SIZE32KB; start < end - SIZE32KB + SIZE32KB/W ; start+=ATTACKER_DISTANCE) {

            atk_args->index_min = start;
            atk_args->index_max = end;
            atk_args->array = V;

            if (pthread_create( &atk, NULL, create_attacker, atk_args) != 0)
                printf("\n__Unable to create Attacker's thread");
            pthread_join( atk, NULL);

            fprintf(logfile,"%d \t%d \t%d\n",end, start, (int) atk_args->result);
        }
    }
    
    fclose(logfile);
    free(atk_args);

    return EXIT_SUCCESS;
}




void * create_attacker(void * arg){

    atk_args * p = (atk_args*) arg;
    clock_t start, end;
    double * result_set = malloc( TRIES * sizeof(double));
    int try = 0;
    register int repetitions = 0;
    register int index = 0;    
    register int index_min = p->index_min; 
    register int index_max = p->index_max;
    register char * used_array = p->array;


    for ( try=0; try<TRIES; try++ ){

        start = clock();
        
        for (repetitions = 0; repetitions <= REPETITIONS ; repetitions++)
                for(index = index_min; index < index_max; index+=STRIDE) { 
                    used_array[index] += 1;
                }
                    
        end = clock();

        *(result_set + try) = (double) end - start;
    }
    p->result = average(result_set);


    free(result_set);

}



void * create_victim(void * arg) {


    register int index = 0;

    for (;;)
        for (index = VICTIM_START; index < SIZE32KB; index += STRIDE) {
            
            V[index] += 1;

        }
    return NULL;
}


double average(double * list) {

    double result = 0;
    for (int i = 0; i<TRIES; i++){
        result+= *(list+i); // WARNING! Operation might overflow
    }
    result = result/TRIES;
    return result;
}

