CC=g++
CFLAGS=-shared -fPIC
CSOURCES=c++/papi_jni.c
OUTPUT_SO=libpapitry.so
JAVASSIST=./javassist/javassist-3.16.1-GA/javassist.jar

all:
	$(CC) $(CFLAGS) -I/usr/lib/jvm/java-7-oracle/include -I/usr/lib/jvm/java-7-oracle/include/linux -I/usr/local/include $(CSOURCES) -L/usr/local/lib -lpapi -Wl,-rpath,/usr/local/lib -o $(OUTPUT_SO)

clean:
	rm -rf $(OUTPUT_SO)

java:
	rm -rf papiJ.class ProfilerMain.class ProfilerTransformer.class
	javac -classpath .:$(JAVASSIST) papiJ.java ProfilerTransformer.java ProfilerMain.java

jar:
	#jar cfm papiJ_jar.jar Manifest.txt papiJ.class
	rm -rf profiler.jar
	jar cfm profiler.jar Manifest.txt ProfilerMain.class ProfilerTransformer.class
