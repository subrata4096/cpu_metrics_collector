
import java.lang.*;

public class papiJ {

/** Current version number **/
public static final int PAPI_VER_CURRENT = 83886080;

public native int PAPI_JNI_library_init(int i);
public native int PAPI_JNI_threads_init(long tid);

public native int PAPI_JNI_start(int i);
public native int PAPI_JNI_stop(int i);

public int PAPIJ_library_init(int i)
{
   System.out.println("PAPIJ_library_init called ");
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

public int PAPIJ_start(int i)
{
  System.out.println("PAPIJ_start called ");
   int ret = PAPI_JNI_start(i);
   return ret;
} 

public int PAPIJ_stop(int i)
{
 System.out.println("PAPIJ_stop called ");

   int ret = PAPI_JNI_stop(i);
   return ret;
} 

static {
    System.out.println("loading...");

    System.loadLibrary("papitry");
  }

}
