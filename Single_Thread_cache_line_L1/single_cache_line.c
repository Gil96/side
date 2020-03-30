/* 
    >>  make
    >>  ./single
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

double average(double * list);


int main(void) {


    FILE* logfile = fopen(LOGFILE,"w");
    int try = 0;
    register int repetitions = 0;
    register int index = 0;
    register int max;
    register int min;
    clock_t start, end;
    double * result_set = malloc( TRIES * sizeof(double));



    for ( max = SIZE32KB; max <= SIZE64KB; max = max*2 ) {
        for ( min = max - SIZE32KB; min < max - SIZE32KB + SIZE32KB/W ; min+=ATTACKER_DISTANCE) {
            for ( try=0; try<TRIES; try++ ){

                start = clock();
                for (repetitions = 0; repetitions <= REPETITIONS ; repetitions++) {
                    for(index = min; index < max; index+=STRIDE) { 
                        V[index] += 1; 
                    }

                    for(index=VICTIM_START; index < SIZE32KB; index += STRIDE) {
                        V[index] += 1;
                    }
                }
                end = clock();

                *(result_set + try) = (double) end - start;

            }
            fprintf(logfile,"%d \t%d \t%d\n", max, min, (int) average(result_set));
        }
    }

    fclose(logfile);

    return EXIT_SUCCESS;
}



double average(double * list) {

    double result = 0;
    for (int i = 0; i<TRIES; i++){
        result+= *(list+i); // Warning! operation might overflow
    }
    result = result/TRIES;
    return result;
}

