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
# define NUM_EVENTS 3
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
  int i, j;
  for (i = 0; i < 100; i++)
  {
    for (j = 0; j < 100; j++)
	{
	   g();
	 }
   }

   return 1;

}



int main(int argc, char **argv) {
  extern void dummy(void *);
  float real_time, proc_time, mflops;
  long long flpops;
  //int Events [5]  = {PAPI_BR_MSP, PAPI_TOT_IIS,PAPI_HW_INT,PAPI_L2_LDM,PAPI_L2_STM} ;
  int Events [NUM_EVENTS]  = {PAPI_HW_INT , PAPI_BR_MSP,PAPI_L2_LDM} ;
  int   EventSet = PAPI_NULL ;
  unsigned int native = 0x0;
  long long flpins;
  int retval;
  int i,k,max_to_add = 6, j = 0;;
  long long values[NUM_EVENTS],values1[7],values2[3];
 long long * val;
  //int eventcodes[7] = {};

  retval = PAPI_library_init(PAPI_VER_CURRENT);
  if(retval != PAPI_VER_CURRENT)
  {
   cout << "error\n";
   exit(1);
  }
retval = PAPI_multiplex_init();
  if(retval != PAPI_OK)
  {
   cout << "error in multiplexing\n";
   exit(1);
  }
  
  PAPI_set_debug(PAPI_VERB_ECONT);
  int ret = -12;
  ret = PAPI_create_eventset(&EventSet);
  if(ret == PAPI_OK)
  {
    cout << "event set created\n";
  }
  else
  {
    cout << "event set failed: " << ret << "\n";
  }
   PAPI_assign_eventset_component(EventSet, 0) ;
  ret = PAPI_set_multiplex(EventSet);
  if(ret  != PAPI_OK)
  { 
    cout << "multiplexing failed: " << ret << "\n";
  }
   ret = PAPI_get_multiplex(EventSet);
  
    cout << ret << "\n";
  
  cout <<  PAPI_EINVAL << " " <<  PAPI_ENOMEM << " "  << PAPI_ENOEVST << " "  << PAPI_EISRUN << " " <<  PAPI_ECNFLCT << " " << PAPI_ENOEVNT <<  " " << PAPI_EBUG << "\n";

                

		ret = PAPI_add_events(EventSet, Events,NUM_EVENTS) ;
		if (ret != PAPI_OK)
		{
			cout << "ret Adding " << ret  << "\n";
		}

int x =1;
 PAPI_start(EventSet);
 f(x);
 PAPI_read(EventSet,values);
 for (int k=0;k<max_to_add;k++)
 {
   cout << "vlues: " << k << " := " << values[k] << "\n";
 }
 PAPI_stop(EventSet,values);
 
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

