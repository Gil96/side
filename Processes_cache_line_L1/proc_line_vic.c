/* 
    
    >>  make
    >>  ./vic
    >>  ./atk
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


#define SIZE32KB (32*1024)        //  represents 32 KB
#define SIZE64KB (64*1024)        //  represents 64 KB
#define W 8                       //  associativity number of L1
#define STRIDE (SIZE32KB/W)       //  step distance between the consecutive accesses in order to fill a particular line of L1
#define VICTIM_START 1024         //  victim's first position to access in the array [0-4KB]

char V[SIZE64KB];                 //  array to be accessed


int main(void) {

    register int index = 0;

    for(;;)
        for(index=VICTIM_START; index < SIZE32KB; index += STRIDE) {
            
            V[index] += 1;

        }
}
