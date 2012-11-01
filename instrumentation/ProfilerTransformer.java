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
                p = new papiJ();

		classPool = ClassPool.getDefault();
		
		pattern = System.getenv("PROFILE_PATTERN");
		if (pattern == null) {
			System.err.println("Could not find variable 'PROFILE_PATTERN'.");
			System.exit(1);
		} else {
			pattern = pattern.replace(".","/");
		}
		
		System.out.println("*************************************************");
		System.out.println("* In Java profiler");
		System.out.println("* Pattern: " + pattern);
		System.out.println("*************************************************");
	}
	
	public byte[] transform(
			ClassLoader loader, 
			String className, 
			Class<?> classBeingRedefined,
			ProtectionDomain protectionDomain, 
			byte[] classfileBuffer)
	throws IllegalClassFormatException
	{
		if (!className.contains(pattern)) {
			return classfileBuffer;
		}
           System.out.println("FOUND  " + className);
		
		classPool.insertClassPath(new ByteArrayClassPath(className, 
				classfileBuffer));
		CtClass cc = null;
		try {
			String name = className.replace("/",".");
			cc = classPool.get(name);
		} catch (NotFoundException nfe) {
			System.out.println("NotFoundException: " + nfe.getMessage() + 
					"; transforming class " + className + 
					"; returning uninstrumented class");
			nfe.printStackTrace();
		}
		
		//print("In transformer. class name: " + className);
		if (cc.isInterface() == false) {
			for (CtMethod m : cc.getDeclaredMethods()) {
				try {

					String methodName = m.getName();
					if(methodName.contains("main")) 
					{ 
                                                continue;
						//m.insertAfter(" { ProfilerTransformer.getInstance().p.PAPIJ_library_init(papiJ.PAPI_VER_CURRENT); }");
					} 
					String callName = className + "$" + m.getName();
					System.out.println("call name is: " + callName);
					m.insertBefore("{ ProfilerTransformer.getInstance().p.PAPIJ_start(0); }");
					m.insertAfter("{ ProfilerTransformer.getInstance().p.PAPIJ_stop(0); }");
                                        //m.insertAfter( " System.out.println(\"wow instrumented\") ; " );
				} catch (CannotCompileException cce) {
					System.out.println("CannotCompileException: " + 
					  cce.getMessage() + "; instrumenting method " + 
					  m.getLongName() + 
					  "; method will not be instrumented");
				}
			}
			// return the new bytecode array:
			byte[] newClassfileBuffer = null;

			try {
				newClassfileBuffer = cc.toBytecode();
                                //cc.writeFile();
                                String cs = cc.toString();
                                System.out.println(cs);
			}
			catch (IOException ioe) {
				System.out.println("IOException: " + ioe.getMessage() + 
						"; transforming class " + className + 
						"; returning uninstrumented class");
				return null;
			}
			catch (CannotCompileException cce) {
				System.out.println("CannotCompileException: " + 
						cce.getMessage() + "; transforming class " + 
						className + "; returning uninstrumented class");
				return null;
			}
                        
			
			return newClassfileBuffer;
		} else {
			return classfileBuffer;
		}

	}
	
	public void entryCall(String callName) {
		String id = "ENTER-" + 
		//Thread.currentThread().getName() + "-" + 
		callName;
		//print("***Wrapper: " + id);
	}
	
	public void exitCall(String callName) {
		String id = "EXIT-" + 
		//Thread.currentThread().getName() + "-" + 
		callName;
		//print("***Wrapper: " + id);
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
	
	public void saveData() {
		//mc.saveData();
	}
}

