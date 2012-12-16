#!/bin/bash
export BASEDIR=/home/mitra4/research/performance_counter/PAPI/instrumentation
export JAVASSIST=$BASEDIR/javassist/javassist-3.16.1-GA/javassist.jar
export PROFILE_PATTERN=papi_jni_main
export LD_LIBRARY_PATH=$BASEDIR:$LD_LIBRARY_PATH
#make clean
#make all
#make java
#make jar

#java -classpath .:$JAVASSIST:./profiler.jar -Djava.library.path=/home/mitra4/research/performance_counter/PAPI/instrumentation -javaagent:./profiler.jar papi_jni_main

java -classpath .:$JAVASSIST:$BASEDIR/profiler.jar -Djava.library.path=. -javaagent:$BASEDIR/profiler.jar papi_jni_main
