import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.lang.instrument.Instrumentation;
import java.io.IOException;
import java.security.ProtectionDomain;
import javassist.CannotCompileException;
import javassist.ClassPool;
import javassist.CtClass;
import javassist.ByteArrayClassPath;
import javassist.CtMethod;
//import javassist.CtBehavior;
import javassist.NotFoundException;

public class ProfilerTransformer implements ClassFileTransformer
{
	protected static ProfilerTransformer profilerInstance = null;
	protected Instrumentation instrumentation = null;
	protected ClassPool classPool;
	protected boolean off = false;
	protected String pattern = null;
	public papiJ p = null;

	public static ProfilerTransformer getInstance()
	{
		return profilerInstance;
	}

	public ProfilerTransformer()
	{
		profilerInstance = this;
                //attachShutDownHook();
		p = new papiJ();
		p.PAPIJ_library_init(papiJ.PAPI_VER_CURRENT);

		classPool = ClassPool.getDefault();

		pattern = System.getenv("PROFILE_PATTERN");
		if (pattern == null) {
			System.err.println("Could not find variable 'PROFILE_PATTERN'.");
			System.exit(1);
		} else {
			pattern = pattern.replace(".","/");
			//pattern = pattern;
		}

		System.out.println("*************************************************");
		System.out.println("* In Java profiler");
		System.out.println("* Pattern: " + pattern);
		System.out.println("*************************************************");
	}
        public void dumpMetrics()
        {
           ProfilerTransformer.getInstance().p.PAPIJ_dumpMetrics();
        }

	public void attachShutDownHook() {

                AddShutdownHook sample = new AddShutdownHook();
                sample.attachShutDownHook();
/*
		Runtime.getRuntime().addShutdownHook( new Thread() {
				public void run() {
				System.out.println("Inside Add Shutdown Hook");
                                dumpMetrics();
                                }

			});
*/
	}	
	


	public byte[] transform(
			ClassLoader loader, 
			String className, 
			Class<?> classBeingRedefined,
			ProtectionDomain protectionDomain, 
			byte[] classfileBuffer)
		throws IllegalClassFormatException
		{

                          if (className.contains("java/lang/Shutdown") && !off) {
		        	off = true;
			  //Save data in file
			  //mc.saveData();
                          //ProfilerTransformer.getInstance().p.PAPIJ_dumpMetrics();
	          	}

	         	if (off) {
		        	//System.out.println("AFTER_SHUTDOWN: " + className);
			     return classfileBuffer;
	        	}

			//System.out.println("class name :" + className);
			if (!className.contains(pattern)) {
				//System.out.println("NOT FOUND  " + className);
				//System.out.println("------ Pattern: " + pattern + ", Class: " + className);	
				return classfileBuffer;
			}

			if(className.contains("Proxy"))
			{
				return classfileBuffer;
			}                 

			classPool.insertClassPath(new ByteArrayClassPath(className, 
						classfileBuffer));
			String name = className.replace("/",".");
			CtClass cc = null;
			try {
				//String name = className;
				cc = classPool.get(name);
				//cc =  classPool.makeClass(name);
			} catch (NotFoundException nfe) {
				System.out.println("NotFoundException: " + nfe.getMessage() + 
						"; transforming class " + className + 
						"; returning uninstrumented class");
				nfe.printStackTrace();
				return classfileBuffer;
			}
			catch (Exception e) {
				System.out.println("Could not instrument  " + name
						+ ",  exception : " + e.getMessage());
				return classfileBuffer;
			}

			if(cc == null) 
			{
				System.out.println("Could not find class");
				return classfileBuffer;
			}

			//print("In transformer. class name: " + className);
			if (cc.isInterface() == false) {
				//System.out.println("not an interface");
				for (CtMethod m : cc.getDeclaredMethods()) {
					try {

						String methodName = m.getName();
						// System.out.println("method name is: " + methodName);
						String callName = className + "::" + m.getName();

						if(methodName.equals("main")) 
						{ 
							// System.out.println("class name :" + className);
							//  System.out.println("call name is: " + callName);
							//m.insertAfter("throw new Exception();");

							//m.insertAfter(" { ProfilerTransformer.getInstance().p.PAPIJ_library_init(papiJ.PAPI_VER_CURRENT); }");
							continue;
						} 
						//	System.out.println(" _________________ Instrumenting: call name is: " + callName);
						m.insertBefore("{ ProfilerTransformer.getInstance().entryCall(\"" + callName + "\"); }");
						m.insertAfter("{ ProfilerTransformer.getInstance().exitCall(\"" + callName + "\"); } ");
						//m.insertAfter( " System.out.println(\"wow instrumented\") ; " );
					} 
					catch (CannotCompileException cce) {
						System.out.println("CannotCompileException: " + 
								cce.getMessage() + "; instrumenting method " + 
								m.getLongName() + 
								"; method will not be instrumented");
					}
					catch(Exception e)
					{
					}
				}
				// return the new bytecode array:
				byte[] newClassfileBuffer = null;


				try {
					newClassfileBuffer = cc.toBytecode();
					//cc.writeFile();
					//String cs = cc.toString();
					//System.out.println(cs);
				}
				catch (IOException ioe) {
					System.out.println("IOException: " + ioe.getMessage() + 
							"; transforming class " + className + 
							"; returning uninstrumented class");
					return classfileBuffer;
				}
				catch (CannotCompileException cce) {
					System.out.println("CannotCompileException: " + 
							cce.getMessage() + "; transforming class " + 
							className + "; returning uninstrumented class");
					return classfileBuffer;
				}
				catch (Exception e) {
					System.out.println("Could not instrument  " + className
							+ ",  exception : " + e.getMessage());
					return classfileBuffer;
				}



				return newClassfileBuffer;
			} else {
				return classfileBuffer;
			}

		}

	public void entryCall(String callName) {
		//String id = "ENTER-" + 
			//Thread.currentThread().getName() + "-" + 
		//	callName;
		//print("***Wrapper: " + id);
                ProfilerTransformer.getInstance().p.PAPIJ_start(callName);
	}

	public void exitCall(String callName) {
		//String id = "EXIT-" + 
			//Thread.currentThread().getName() + "-" + 
		//	callName;
		//print("***Wrapper: " + id);
               ProfilerTransformer.getInstance().p.PAPIJ_stop(callName);
	}

	/*private void print(String str) {
	  System.out.println(str);
	  }*/

	/*private boolean isJavaMethod(String call) {
	  return (call.charAt(0)=='j' && 
	  call.charAt(1)=='a' && 
	  call.charAt(2)=='v' && 
	  call.charAt(3)=='a' &&
	  call.charAt(4)=='/');
	  }*/
	/*
	   public void saveData() {
//mc.saveData();
} */
}

