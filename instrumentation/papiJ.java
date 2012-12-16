
import java.lang.*;

public class papiJ {

/** Current version number **/
public static final int PAPI_VER_CURRENT = 83886080;

public native int PAPI_JNI_library_init(int i);
public native int PAPI_JNI_threads_init(long tid);

public native int PAPI_JNI_start(String methodName);
public native int PAPI_JNI_stop(String methodName);
public native void PAPI_JNI_dumpMetrics();

public int PAPIJ_library_init(int i)
{
   System.out.println("PAPIJ_library_init called ");
//int ret = -1;   
int ret = PAPI_JNI_library_init(i);
   return ret;
}

public native long f();

public int PAPIJ_threads_init(int i)
{
   System.out.println("PAPIJ_threads_init called ");
  // will not use i
   //long tid = Thread.currentThread().getId();
   //int ret = PAPI_JNI_threads_init(Thread.currentThread().getId);
   return -1;
} 

public int PAPIJ_start(String methodName)
{
  System.out.println("PAPIJ_start called ");
 // int ret = -1;
   int ret = PAPI_JNI_start(methodName);
   return ret;
} 

public int PAPIJ_stop(String methodName)
{
 System.out.println("PAPIJ_stop called ");
  // int ret = -1;
   int ret = PAPI_JNI_stop(methodName);
   return ret;
}

public void PAPIJ_dumpMetrics()
{
  PAPI_JNI_dumpMetrics();
} 

static {
    System.out.println("loading...");

    System.loadLibrary("papitry");
  }

}
