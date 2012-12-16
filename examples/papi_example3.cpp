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
  float real_time, proc_time, mflops;
  long long flpops;
  int EventSet1 = PAPI_NULL, EventSet2 = PAPI_NULL;
  unsigned int native = 0x0;
  long long flpins;
  int retval;
  int i,j,k;
  long long values1[7],values2[3];
  //int eventcodes[7] = {};

  retval = PAPI_library_init(PAPI_VER_CURRENT);
  if(retval != PAPI_VER_CURRENT)
  {
   cout << "error\n";
   exit(1);
  }
  if(PAPI_create_eventset(&EventSet1) == PAPI_OK)
  {
    cout << "event set created\n";
  }
  if(PAPI_add_event(EventSet1,PAPI_BR_MSP)== PAPI_OK)
  {
     cout << "Event PAPI_BR_MSP added \n";
  }
  if(PAPI_add_event(EventSet1,PAPI_BTAC_M)== PAPI_OK)
  {
     cout << "Event PAPI_BTAC_M added \n";
  }
  if(PAPI_add_event(EventSet1,PAPI_FAD_INS)== PAPI_OK)
  {
     cout << "Event PAPI_FAD_INS added \n";
  }
  if(PAPI_add_event(EventSet1,PAPI_TOT_IIS)== PAPI_OK)
  {
     cout << "Event PAPI_TOT_IIS added \n";
  }
  if(PAPI_add_event(EventSet1,PAPI_HW_INT)== PAPI_OK)
  {
     cout << "Event PAPI_HW_INT added \n";
  }
  if(PAPI_add_event(EventSet1,PAPI_L2_LDM)== PAPI_OK)
  {
     cout << "Event PAPI_L2_LDM added \n";
  }
  if(PAPI_add_event(EventSet1,PAPI_L2_STM)== PAPI_OK)
  {
     cout << "Event PAPI_L2_STM added \n";
  }
  if(PAPI_add_event(EventSet2,PAPI_RES_STL_idx)== PAPI_OK)
  {
     cout << "Event PAPI_RES_STL_idx added \n";
  }
  if(PAPI_add_event(EventSet2,PAPI_HW_INT_idx)== PAPI_OK)
  {
     cout << "Event PAPI_HW_INT_idx added \n";
  }
  if(PAPI_add_event(EventSet2,PAPI_MEM_RCY) == PAPI_OK)
  {
     cout << "Event PAPI_MEM_RCY added \n";
  }

int x =1;
 PAPI_start(EventSet1);
 f(x);
 PAPI_read(EventSet1,values1);
 for (int k=0;k<7;k++)
 {
   cout << "vlues: " << k << " := " << values1[k] << "\n";
 }
 PAPI_stop(EventSet1,values1);
 
PAPI_start(EventSet1);
 PAPI_start(EventSet2);
 f(x);
 PAPI_read(EventSet1,values1);

 PAPI_read(EventSet2,values2);
 for (int k=0;k<7;k++)
 {
   cout << "vlues: " <<  k << " := " << values1[k] << "\n";
 }
 for (int k=0;k<7;k++)
 {
   cout << "vlues2 " << k << " := " << values2[k] << "\n";
 }
 PAPI_stop(EventSet1,values1);
 PAPI_stop(EventSet2,values2);
 
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

