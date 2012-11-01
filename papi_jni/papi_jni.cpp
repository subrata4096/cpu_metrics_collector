#include <iostream>
#include "papiJ.h"
#include <papi.h> 

using namespace std;

JNIEXPORT jint JNICALL Java_papiJ_PAPI_library_init
  (JNIEnv *env, jobject obj)
{
  jint ret = 100;
  //int ret = PAPI_library_init(ver);
  cout << " calling from interface " << ret << "\n";
  //return ret;
}

