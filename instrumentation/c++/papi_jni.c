#include <iostream>
#include <pthread.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

#include <papi.h>
#include "papiJ.h"
#include <map>
#include <list>

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
#define THRESHOLD 10000
static int EventSet;

static long_long values[NUMEVENTS];
static double dvalues[NUMEVENTS];
static map<string,bool> methodNameOutputFileMap;
static map<string,string> metricsMap;
static list<string> metricsList;
void writeResults();

class dumpClass
{
  public:
  int* i;
  dumpClass()
  {
     cout << " *** constructor called " << "\n";
    i = new int(0);
  }
  ~dumpClass()
  {
     cout << " *** destructor called " << "\n";
     writeResults();
     delete i;
  }

};

static dumpClass scopeInstance;

void handler(int EventSet, void *address, long_long overflow_vector, void *context)
{
	int i;

	/* Stop counting events */
	if (PAPI_read(EventSet, values) != PAPI_OK)
	{ 
		printf("PAPI handler read Error\n");
		return ;
	}

	//fprintf(stderr, "handler(%d) Overflow at %p! vector=0x%llx\n",
	//		EventSet, address, overflow_vector);

	for(i = 0; i < NUMEVENTS; i++) {
		dvalues[i] += i ? (double)values[i] : THRESHOLD;
		/* printf("%15.2lf %15lu\n",dvalues[i], (unsigned long int)values[i]); */
	}

	if (PAPI_reset(EventSet) != PAPI_OK)
	{ 
		printf("PAPI handler reset Error\n");
		return ;
	}

}

string checkFileName(string fName, int i)
{
         stringstream ss;
	map<string,bool> :: iterator fPos = 
                             methodNameOutputFileMap.find(fName); 
	if(fPos != methodNameOutputFileMap.end())
	{
		ss << fName <<  "_" << i;
	}
        else
        {
             methodNameOutputFileMap[fName] = true;
             return fName;
        }
	
        string fModified = string(ss.str());
        return checkFileName(fModified,(i+1));
}
void print_results(const char* methodName)
{
        //string dumpFileName = string(methodName);
        //dumpFileName = checkFileName(dumpFileName,1);

        char eventName[PAPI_MAX_STR_LEN];
	int i;
        stringstream ss;
        ss << string(methodName) << "," ;
	for(i = 0; i < NUMEVENTS; i++) {
		if (PAPI_event_code_to_name(Events[i], eventName) != PAPI_OK)
		{
			printf("PAPI event code to name Error\n");
			//return;
                        continue;
		}
	//	printf("  Event %15s: %15.0lf\n", eventName, dvalues[i]);
                ss << dvalues[i] << ",";  
	}
        //ss << "\n";
        //metricsMap[dumpFileName] = string(ss.str());
        metricsList.push_back(string(ss.str()));
        
       // printf("\n ***** HERE **  \n");
       // cout << ss.str() << "\n";
        //fstream myFile;
        //string fName = "/home/mitra4/" + dumpFileName;
        //myFile.open(fName.c_str());
        //myFile << ss.str();
        //myFile.close();
}

void writeResults()
{
  string outputFileName = "/home/mitra4/output_papi_results.txt";
   ofstream myFile;
   myFile.open(outputFileName.c_str());
  char eventName[PAPI_MAX_STR_LEN];
  int i;
   myFile << "ID,";
   for(i = 0; i < NUMEVENTS; i++) {
                if (PAPI_event_code_to_name(Events[i], eventName) != PAPI_OK)
                {
                        printf("PAPI event code to name Error\n");
                        continue;
                }
                myFile << eventName << ",";
        }
    myFile << "\n";

   //map<string,string>::iterator ii = metricsMap.begin();
   //map<string,string>::iterator jj = metricsMap.end();
   list<string>::iterator ii = metricsList.begin();
   list<string>::iterator jj = metricsList.end();
   for(;ii != jj; ii++)
{
   
   //myFile << (*ii).first << "\n";
   //myFile << (*ii).second << "\n";
   myFile << (*ii) << "\n";
   //cout << (*ii).first << "\n";
   //cout << (*ii).second << "\n";
}

   myFile.close();

}


	JNIEXPORT jint JNICALL Java_papiJ_PAPI_1JNI_1library_1init
(JNIEnv *env, jobject obj,jint ver)
{
        int* scope = scopeInstance.i;
        cout << *(scope) << "\n";
	jint ret = -1;
	EventSet = PAPI_NULL;
	ret = PAPI_library_init(ver);
	cout << " calling from interface " << ver << " and " << ret << "\n";
	if(ret == -1)
	{ 
		printf("PAPI library init Error\n");
		//return ret;
                exit(0);
	}

	PAPI_multiplex_init();

        if (PAPI_thread_init(pthread_self) != PAPI_OK)
        { 
         	printf("PAPI thread init Error\n");
		//return -1;
                exit(0);
        }
/*
	int num_hwcntrs = 0;
	if ((num_hwcntrs = PAPI_num_counters()) <= PAPI_OK)
	{
		printf("PAPI hw count Error\n");
		return -1;
	}
	printf("Hw counters:%d\n",num_hwcntrs);
*/

	if(PAPI_create_eventset(&EventSet) != PAPI_OK)
	{  
		printf("PAPI create eventset Error\n");
		//return -1;
                exit(0);
	}
        //int thread_id =  pthread_self(); 
        //        printf("PAPI thread id is: %d\n",thread_id);
	printf("eventset created\n");

	if (PAPI_assign_eventset_component(EventSet, 0) != PAPI_OK)
	{
		printf("PAPI assign eventset Error\n");
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
			printf("PAPI add event Error\n");
			//return -1;

		}
		printf("Adding even %d : %d   Error = %d\n",i,Events[i],ret);
#if 0 
                ret = PAPI_overflow(EventSet, Events[i], THRESHOLD,0 , handler);
                if(ret != PAPI_OK)
                {
                  printf("Error in adding overflow for %d with error %d\n",i,ret);
                  continue;
                }
#endif 

	}
	ret = PAPI_get_multiplex(EventSet);
	printf("get mutiplex event ret val: %d\n",ret);


//	printf("This system has %d available counters.\n", num_hwcntrs);
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
(JNIEnv * env, jobject obj, jstring methodName)
{
        //return -1;
        jint ret = PAPI_OK;
         int xRC, xState;
        xRC = PAPI_state(EventSet, &xState);
       if (xRC == PAPI_OK)
       {

         //if(xState == (PAPI_RUNNING | PAPI_MULTIPLEXING))
         if(xState != 65)
        {
          return -1;
        }
      }
      //cout << "xState:" << xState << "\n";
const char *str= (env)->GetStringUTFChars(methodName,0);
         //int thread_id =  pthread_self(); 
             //   printf("PAPI thread id is: %d\n",thread_id);
int papi_ret = PAPI_start(EventSet);
	if (papi_ret != PAPI_OK)
	{
		cout << "PAPI start Error : " << papi_ret << " : "  << str << "\n";
	(env)->ReleaseStringUTFChars(methodName, str);
		return -1;
	}
        else
{
//  printf("PAPI start Good\n");

}
	(env)->ReleaseStringUTFChars(methodName, str);
        return ret;
}

	JNIEXPORT jint JNICALL Java_papiJ_PAPI_1JNI_1stop
(JNIEnv *env , jobject obj, jstring methodName)
{
       //return -1;
        const char *str= (env)->GetStringUTFChars(methodName,0);
        jint ret = PAPI_OK;
        int xRC, xState;
        xRC = PAPI_state(EventSet, &xState);
       if (xRC == PAPI_OK)
       {

         //if(xState == (PAPI_STOPPED | PAPI_MULTIPLEXING))
         if(xState != 66)
        {
               int papi_ret = PAPI_read(EventSet, values);
               if (papi_ret != PAPI_OK)
               {
                 cout <<  "PAPI read Error: " << papi_ret << " : " << str << "\n";
                
               }
               else
               {
                   for(int i = 0; i < NUMEVENTS; i++) {
                       dvalues[i] += (double)values[i];
                   }

	           print_results(str);
               }
	       (env)->ReleaseStringUTFChars(methodName, str);
               return -1;
         }
      }
        //cout << "xState:" <<xState << "\n";
        //int thread_id =  pthread_self(); 
        //        printf("PAPI thread id is: %d\n",thread_id);
   
        int papi_ret = PAPI_stop(EventSet, values);
	if (papi_ret != PAPI_OK)
	{
		cout <<  "PAPI stop Error: " << papi_ret << " : " << str << "\n";
		return -1;
	}
	for(int i = 0; i < NUMEVENTS; i++) {
		dvalues[i] += (double)values[i];
	}

        string mName(str);
        //cout << "From cpp  ***********" << mName << endl;	
	print_results(str);

        //need to release this string when done with it in order to
	//avoid memory leak
	(env)->ReleaseStringUTFChars(methodName, str);

        return ret;
}

JNIEXPORT void JNICALL Java_papiJ_PAPI_1JNI_1dumpMetrics
  (JNIEnv *, jobject)
{
   writeResults();
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
