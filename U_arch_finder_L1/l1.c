/*
Description:

	program that times the same number of accesses of different positions (given by the stride) in each different sized array (given by array size).
It corresponds to a variation to the code in lab2_kit/cm1 since the number of accesses are the same for each different (array:stride) pair.

	Plotting the results of this program it's possible to recognize u-arch of the system like:

		-> L1-data size
		->Cache block size
		->L1 Associativity number
		and then infer nº of L1 lines


Commands:
    >>  make
    >>  ./l1
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
#include <time.h>

#define N (1024*1024)
#define ARRAY_MIN (8*1024)
#define ARRAY_MAX (256*1024)
#define LOGFILE "results.out"

int main() {

    clock_t start, end;

    register int array_size;
    register int stride;
    register int limit;
    register int repeat;
    register int index;
    char A[N];
    FILE* logfile;
    register int freq = 0;


    logfile=fopen(LOGFILE,"w");
    
    

    for(array_size=ARRAY_MIN; array_size <= ARRAY_MAX; array_size=array_size*2){
        freq = ARRAY_MAX/array_size;

        for(stride=1; stride <= array_size/2; stride=stride*2){ 
            limit=array_size-stride+1;        


            start = clock();
            for(repeat=0; repeat<= 200* freq* stride; repeat++)
                for(index=0; index<limit; index+=stride)
                    A[index] = A[index] + 1;
            end = clock();

            fprintf(logfile,"array_size=%d \tSTRIDE=%d \tavg_TIME=%f\n", array_size, stride, (double) end - start);
        }
    }

    fclose(logfile);
    return 0;
}
