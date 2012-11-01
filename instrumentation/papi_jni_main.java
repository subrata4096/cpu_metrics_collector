public class papi_jni_main {

	int k = 0;

	void useCPU() {
		int s = 1;
		int f = 2000;
		for (int i=0; i < f; i++)
			for (int j=0; j < f; j++)
				for (int k=0; k < f; k++)
					s = s * 2;
		k = s;
	}

	public static void main(String[] args)
	{
                ProfilerTransformer.getInstance().p.PAPIJ_library_init(papiJ.PAPI_VER_CURRENT); 
		try
		{  
			new papi_jni_main().useCPU(); 
		}
		catch(Exception e)

		{
			System.out.println("Caught IOException: " + e.getMessage());
		}
	}

}
