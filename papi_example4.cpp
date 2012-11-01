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
  float real_time, proc_time, mflops;
  long long flpops;
  //int Events [5]  = {PAPI_BR_MSP, PAPI_TOT_IIS,PAPI_HW_INT,PAPI_L2_LDM,PAPI_L2_STM} ;
  int Events [NUM_EVENTS]  = {PAPI_HW_INT , PAPI_BR_MSP,PAPI_L2_LDM} ;
  int   EventSet = PAPI_NULL ;
  unsigned int native = 0x0;
  long long flpins;
  int retval;
  int i,j,k;
  long long values[NUM_EVENTS],values1[7],values2[3];
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
  if(ret = PAPI_create_eventset(&EventSet) == PAPI_OK)
  {
    cout << "event set created\n";
  }
  if(ret = PAPI_set_multiplex(EventSet) != PAPI_OK)
  { 
    cout << ret << "\n";
  }
  if(ret = PAPI_add_events(EventSet,Events, NUM_EVENTS) != PAPI_OK)
  {
     cout << ret << " All Events not added \n";
  }
  if(ret = PAPI_add_event(EventSet,PAPI_TOT_IIS) != PAPI_OK)
  {
     cout << ret << " Event PAPI_TOT_IIS not added \n";
  }

  cout <<  PAPI_EINVAL << " " <<  PAPI_ENOMEM << " "  << PAPI_ENOEVST << " "  << PAPI_EISRUN << " " <<  PAPI_ECNFLCT << " " << PAPI_ENOEVNT <<  " " << PAPI_EBUG << "\n";

#if 0
  if(PAPI_add_event(EventSet1,PAPI_BTAC_M)== PAPI_OK)//not available on mensa
  {
     cout << "Event PAPI_BTAC_M added \n";
  }
#endif
#if 0

  if(PAPI_add_event(EventSet1,PAPI_FAD_INS)== PAPI_OK) //not available on mensa
  {
     cout << "Event PAPI_FAD_INS added \n";
  }
#endif
#if 0
  //int ret = PAPI_add_event(EventSet1,PAPI_TOT_IIS);
  //cout << "add returned value : " << ret << "PAPI oK " << PAPI_EINVAL << "\n";
  if(ret = PAPI_add_event(EventSet1,PAPI_TOT_IIS) != PAPI_OK)
  {
     cout << ret <<" Event PAPI_TOT_IIS not added \n";
  }
  if(ret = PAPI_add_event(EventSet1,PAPI_HW_INT) != PAPI_OK)
  {
     cout << ret << " Event PAPI_HW_INT not added \n";
  }
  if(ret = PAPI_add_event(EventSet2,PAPI_L2_LDM) != PAPI_OK)
  {
     cout << ret << " Event PAPI_L2_LDM not added \n";
  }
  if(ret = PAPI_add_event(EventSet2,PAPI_L2_STM) != PAPI_OK)
  {
     cout << ret << " Event PAPI_L2_STM not added \n";
  }
#endif
#if 0
  if(PAPI_add_event(EventSet2,PAPI_RES_STL_idx)== PAPI_OK)//not available on mensa
  {
     cout << "Event PAPI_RES_STL_idx added \n";
  }
#endif
#if 0
  if(PAPI_add_event(EventSet2,PAPI_HW_INT_idx)== PAPI_OK)//not available on mensa
  {
     cout << "Event PAPI_HW_INT_idx added \n";
  }
#endif
#if 0
  if(PAPI_add_event(EventSet2,PAPI_MEM_RCY) == PAPI_OK)//not available on mensa

  {
     cout << "Event PAPI_MEM_RCY added \n";
  }
#endif

int x =1;
 PAPI_start(EventSet);
 f(x);
 PAPI_read(EventSet,values);
 for (int k=0;k<NUM_EVENTS;k++)
 {
   cout << "vlues: " << k << " := " << values[k] << "\n";
 }
 PAPI_stop(EventSet,values);
 
PAPI_start(EventSet);
// PAPI_start(EventSet2);
 f(x);
 PAPI_read(EventSet,values);

// PAPI_read(EventSet2,values2);
 for (int k=0;k<NUM_EVENTS;k++)
 {
   cout << "vlues: " <<  k << " := " << values[k] << "\n";
 }
#if 0
 for (int k=0;k<7;k++)
 {
   cout << "vlues2 " << k << " := " << values2[k] << "\n";
 }
#endif
 PAPI_stop(EventSet,values);
 //PAPI_stop(EventSet2,values2);
 
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

