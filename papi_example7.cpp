#include <stdlib.h>
#include <stdio.h>
#include <papi.h> 

#define MSIZE 2000
#define THRESHOLD 2000000000

static const char *ErrMsg[] = {
  "Example program",
  "Couldn't find any hardware counters",
  "Couldn't start counting events",
  "Couldn't stop counting events",
  "Couldn't translate event code into event name",
  "Couldn't initialize overflow handler",
  "Couldn't reset event set"
};

static int Events[] = {
  PAPI_TOT_CYC,
  PAPI_TOT_INS,
  PAPI_L1_DCM,
  PAPI_L2_DCM
};

#define NUMEVENTS (sizeof(Events) / sizeof(int))

static long_long values[NUMEVENTS];
static double dvalues[NUMEVENTS];

/* Data for computation */
static double a[MSIZE][MSIZE], b[MSIZE][MSIZE], c[MSIZE][MSIZE];

void handle_error(int errcode)
{
  printf("Execution yielded error code %d : %s\n", errcode, ErrMsg[errcode]);
 // exit(errcode);
}

void print_results()
{ char eventName[PAPI_MAX_STR_LEN];
  int i;

  for(i = 0; i < NUMEVENTS; i++) {
    if (PAPI_event_code_to_name(Events[i], eventName) != PAPI_OK)
      handle_error(4);
    printf("  Event %15s: %15.0lf\n", eventName, dvalues[i]);
  }

}

void handler(int EventSet, void *address, long_long overflow_vector, void *context) 
{ int i;

  /* Stop counting events */ 
  if (PAPI_read(EventSet, values) != PAPI_OK)
    handle_error(3);

  /* fprintf(stderr, "handler(%d) Overflow at %p! vector=0x%llx\n", 
     EventSet, address, overflow_vector); */

  for(i = 0; i < NUMEVENTS; i++) {
    dvalues[i] += i ? (double)values[i] : THRESHOLD;
    /* printf("%15.2lf %15lu\n",dvalues[i], (unsigned long int)values[i]); */
  }

  if (PAPI_reset(EventSet) != PAPI_OK)
    handle_error(6);
} 

int main(int argc, char **argv)
{ int i, j, k, repe = 10, num_hwcntrs = 0; 
  long_long begin_cyc, end_cyc, begin_msec, end_msec;
  int EventSet = PAPI_NULL;

  if( argc > 1 ) {
    repe = atoi(argv[1]);
  }

  printf("Repeat %d times\n", repe);

  if( PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT)
   handle_error(1);
  printf("library initialized\n");
  
  PAPI_multiplex_init();

  /* Initialize the PAPI library and get the number of counters available */ 
  if ((num_hwcntrs = PAPI_num_counters()) <= PAPI_OK)   
    handle_error(1);
  printf("Hw counters:%d\n",num_hwcntrs);

  /* Create the EventSet */ 
  if (PAPI_create_eventset(&EventSet) != PAPI_OK) 
    handle_error(1); 
  printf("eventset created\n");

 if (PAPI_assign_eventset_component(EventSet, 0) != PAPI_OK)
    handle_error(1);

  int ret = PAPI_set_multiplex(EventSet);
  printf("set mutiplex event ret val: %d\n",ret);

  /* add events to our eventset */ 
  for(i = 0; i < NUMEVENTS; i++) {
    dvalues[i] = 0.0;
    ret = PAPI_add_event(EventSet, Events[i]);
    if (ret != PAPI_OK) 
      handle_error(1); 
     printf("Adding even %d : %d   Error = %d\n",i,Events[i],ret);
  }
  ret = PAPI_get_multiplex(EventSet);
  printf("get mutiplex event ret val: %d\n",ret);

  printf("This system has %d available counters.\n", num_hwcntrs);
  int num_events = 1*NUMEVENTS;
  printf("We will count %d events.\n", num_events);

 if(PAPI_overflow(EventSet, Events[0], THRESHOLD, 0, handler) != PAPI_OK) 
    handle_error(5); 

  begin_cyc = PAPI_get_real_cyc();
  begin_msec = PAPI_get_real_usec();

  puts("Counting events...");

  /* Start counting */ 
  if (PAPI_start(EventSet) != PAPI_OK) 
     handle_error(2); 

  for(k = 0; k < repe; k++)
    for(i = 0; i < MSIZE; i++)
      for(j = 0; j < MSIZE; j++) {
	a[i][j] = (double)i;
	b[i][j] = (double)j;
	c[i][j] = a[i][j] + b[i][j];
      }

  /* Stop counting events */ 
  if (PAPI_stop(EventSet, values) != PAPI_OK)
    handle_error(3);

  for(i = 0; i < NUMEVENTS; i++) {
    dvalues[i] += (double)values[i];
    /* printf("%15.2lf %15lu\n",dvalues[i], (unsigned long int)values[i]); */
  }

  end_cyc = PAPI_get_real_cyc();
  end_msec = PAPI_get_real_usec();

  puts("Events done...");

  printf("Wall clock clycles     : %15lld\nCall clock microseconds: %15lld\n",
	 end_cyc - begin_cyc, end_msec - begin_msec);

  print_results();

  return 0;
}
