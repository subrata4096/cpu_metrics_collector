/*
 * A simple example ofthe use of the high level PAPI_flops call.
 * PAPI_flops measures elapsed time, process time, floating point
 * instructions and MFLOP/s for code bracketted by calls to this routine.
 * For the following matrix multiply you should get 2*(INDEX^3) flpins 
 * on Intel Pentium processors.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <memory.h>
#include <malloc.h>
#include "papi.h"
#include<iostream>
#include<fstream>
using namespace std;


#define INDEX 100


static void test_fail(char *file, int line, char *call, int retval);
int h(int i)
{
  ofstream myfile;
  myfile.open ("example.txt");
  myfile << "Writing this to a file.\n";
  myfile.close();
  cout << "counter is: " << i << endl;

}
void g()
{
  char a;
  for(int k=0;k<10;k++)
  {
    if(k%2)
	{
	  a = 'A';
	}
	else
	{
	  a = 'B';
	}
  }
}
int f(int& x)
{
  static char array[100][100];
  int i, j;
  for (i = 0; i < 100; i++)
  {
    for (j = 0; j < 100; j++)
	{
       array[j][i]++;
	   g();
	 }
   }

   return 1;

}



int main(int argc, char **argv) {
  extern void dummy(void *);
  float matrixa[INDEX][INDEX], matrixb[INDEX][INDEX], mresult[INDEX][INDEX];
  float real_time, proc_time, mflops;
  int EventSet = PAPI_NULL;
  unsigned int native = 0x0;
  long long flpins;
  int retval;
  int i,j,k;
int num_hwcntrs;

  retval = PAPI_library_init(PAPI_VER_CURRENT);
  if(retval != PAPI_VER_CURRENT)
{
 printf("PAPI library init error\n");
 exit(1);
}

if(PAPI_create_eventset(&EventSet) != PAPI_OK)
   //handle_error(1);
native = ((0x800000 & 0xffffff) << 8 | (0x01 & 0xff));

if(PAPI_add_event(EventSet, native) != PAPI_OK)
  //handle_error(1);
/*  The installation does not support PAPI */
if ((num_hwcntrs = PAPI_num_counters()) < 0 )
//handle_error(1);
/*  The installation supports PAPI, but has no counters */
if ((num_hwcntrs = PAPI_num_counters()) == 0 )
fprintf(stderr,"Info:: This machine does not provide hardware counters.\n");

num_hwcntrs = PAPI_num_counters();
cout << "number of counters available: " << num_hwcntrs << "\n";

  

  exit(0);
}


static void test_fail(char *file, int line, char *call, int retval)
{

    printf("%s\tFAILED\nLine # %d\n", file, line);
    if ( retval == PAPI_ESYS ) {
        char buf[128];
        memset( buf, '\0', sizeof(buf) );
        sprintf(buf, "System error in %s:", call );
        perror(buf);
    }
    else if ( retval > 0 ) {
        printf("Error calculating: %s\n", call );
    }
    else {
        char errstring[PAPI_MAX_STR_LEN];
        //PAPI_perror(retval, errstring, PAPI_MAX_STR_LEN );
        printf("Error in %s: %s\n", call, errstring );
    }
    printf("\n");
    exit(1);
}

