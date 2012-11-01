#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <papi.h>
#include "papiJ.h"

using namespace std;

static int Events[] = {
 PAPI_L1_DCM,
PAPI_L2_DCM,
PAPI_L2_ICM,
PAPI_L1_TCM,
PAPI_L2_TCM,
PAPI_CA_SHR,
PAPI_CA_CLN,
PAPI_CA_ITV,
PAPI_TLB_DM,
PAPI_TLB_IM,
PAPI_L1_LDM,
PAPI_L1_STM,
PAPI_L2_LDM,
PAPI_L2_STM,
PAPI_HW_INT,
PAPI_BR_CN,
PAPI_BR_TKN,
PAPI_BR_NTK,
PAPI_BR_MSP,
PAPI_BR_PRC,
PAPI_TOT_IIS,
PAPI_TOT_INS,
PAPI_FP_INS,
PAPI_LD_INS,
PAPI_SR_INS,
PAPI_BR_INS,
PAPI_VEC_INS,
PAPI_RES_STL,
PAPI_TOT_CYC,
PAPI_L1_DCH,
PAPI_L1_DCA,
PAPI_L2_DCA,
PAPI_L2_DCR,
PAPI_L2_DCW,
PAPI_L1_ICH,
PAPI_L2_ICH,
PAPI_L1_ICA,
PAPI_L2_ICA,
PAPI_L2_TCH,
PAPI_L1_TCA,
PAPI_L2_TCA,
PAPI_L2_TCR,
PAPI_L2_TCW,
PAPI_FML_INS,
PAPI_FDV_INS,
PAPI_FP_OPS,
PAPI_SP_OPS,
PAPI_DP_OPS,
PAPI_VEC_SP,
PAPI_VEC_DP,
PAPI_REF_CYC

 };

#define NUMEVENTS (sizeof(Events) / sizeof(int))
#define MSIZE 2000
#define THRESHOLD 100000
static int EventSet;

static long_long values[NUMEVENTS];
static double dvalues[NUMEVENTS];

void handler(int EventSet, void *address, long_long overflow_vector, void *context)
{
	int i;

	/* Stop counting events */
	if (PAPI_read(EventSet, values) != PAPI_OK)
	{ 
		printf("Error\n");
		return ;
	}

	fprintf(stderr, "handler(%d) Overflow at %p! vector=0x%llx\n",
			EventSet, address, overflow_vector);

	for(i = 0; i < NUMEVENTS; i++) {
		dvalues[i] += i ? (double)values[i] : THRESHOLD;
		/* printf("%15.2lf %15lu\n",dvalues[i], (unsigned long int)values[i]); */
	}

	if (PAPI_reset(EventSet) != PAPI_OK)
	{ 
		printf("Error\n");
		return ;
	}

}

void print_results()
{ char eventName[PAPI_MAX_STR_LEN];
	int i;

	for(i = 0; i < NUMEVENTS; i++) {
		if (PAPI_event_code_to_name(Events[i], eventName) != PAPI_OK)
		{
			printf("Error\n");
			return;
		}
		printf("  Event %15s: %15.0lf\n", eventName, dvalues[i]);
	}

}


	JNIEXPORT jint JNICALL Java_papiJ_PAPI_1JNI_1library_1init
(JNIEnv *env, jobject obj,jint ver)
{
	jint ret = -1;
	EventSet = PAPI_NULL;
	ret = PAPI_library_init(ver);
	cout << " calling from interface " << ver << " and " << ret << "\n";
	if(ret == -1)
	{ 
		printf("Error\n");
		return ret;
	}

	PAPI_multiplex_init();
	int num_hwcntrs = 0;
	if ((num_hwcntrs = PAPI_num_counters()) <= PAPI_OK)
	{
		printf("Error\n");
		return -1;
	}
	printf("Hw counters:%d\n",num_hwcntrs);

	if(PAPI_create_eventset(&EventSet) != PAPI_OK)
	{   
		printf("Error\n");
		return -1;
	}
	printf("eventset created\n");

	if (PAPI_assign_eventset_component(EventSet, 0) != PAPI_OK)
	{
		printf("Error\n");
		return -1;
	}
	ret = PAPI_set_multiplex(EventSet);
	printf("set mutiplex event ret val: %d\n",ret);


	/* add events to our eventset */
	for(int i = 0; i < NUMEVENTS; i++) {
		dvalues[i] = 0.0;
		ret = PAPI_add_event(EventSet, Events[i]);
		if (ret != PAPI_OK)
		{
			printf("Error\n");
			//return -1;

		}
		printf("Adding even %d : %d   Error = %d\n",i,Events[i],ret);
#if 0
                if(PAPI_overflow(EventSet, Events[i], THRESHOLD, 0, handler) != PAPI_OK)
                {
                  printf("Error\n");
                  return -1;
                }
#endif

	}
	ret = PAPI_get_multiplex(EventSet);
	printf("get mutiplex event ret val: %d\n",ret);


	printf("This system has %d available counters.\n", num_hwcntrs);
	int num_events = 1*NUMEVENTS;
	printf("We will count %d events.\n", num_events);


	return ret;
}

JNIEXPORT jint JNICALL Java_papiJ_PAPI_1JNI_1threads_1init
  (JNIEnv *env, jobject obj, jlong tid)
{
	jint ret = PAPI_OK;
#if 0
        //long unsigned int papi_tid = (long unsigned int)tid;
        if (PAPI_thread_init(Java_papiJ_f) != PAPI_OK)
        {
          ret = -1;
        }
	return ret;
#endif
}

	JNIEXPORT jint JNICALL Java_papiJ_PAPI_1JNI_1start
(JNIEnv * env, jobject obj, jint event_set_num)
{
        jint ret = PAPI_OK;
	if (PAPI_start(EventSet) != PAPI_OK)
	{
		printf("Error\n");
		return -1;
	}
        return ret;
}

	JNIEXPORT jint JNICALL Java_papiJ_PAPI_1JNI_1stop
(JNIEnv *env , jobject obj, jint event_set_num)
{
        jint ret = PAPI_OK;
	if (PAPI_stop(EventSet, values) != PAPI_OK)
	{
		printf("Error\n");
		return -1;
	}
	for(int i = 0; i < NUMEVENTS; i++) {
		dvalues[i] += (double)values[i];
	}

	print_results();
        return ret;
}

JNIEXPORT jlong JNICALL Java_papiJ_f
  (JNIEnv *env, jobject obj1)
{
  jclass cls = env->FindClass("java/lang/Thread");
  jmethodID mid = env->GetStaticMethodID(cls, "currentThread", "(V)Ljava/lang/Thread;");
   if (mid == 0)
    return -1;
  jobject curr_thread = env->CallStaticObjectMethod(cls, mid);
  jmethodID mid2 = env->GetMethodID(cls, "getId", "(V)J");
   if (mid2 == 0)
    return -1;
  jlong id = env->CallLongMethod(curr_thread, mid2);

  printf("thread id is: %ld \n", id);

  return id;
}
