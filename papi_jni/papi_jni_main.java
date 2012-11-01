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
		try
		{  
			papiJ p = new papiJ();

			int retVar = p.PAPIJ_library_init(p.PAPI_VER_CURRENT);
			System.out.println(retVar);
			retVar = p.PAPIJ_start(0);
			// System.out.println(retVar);

			//code under test
			new papi_jni_main().useCPU(); 

			retVar = p.PAPIJ_stop(0);
			System.out.println(retVar);
		}
		catch(Exception e)

		{
			System.out.println("Caught IOException: " + e.getMessage());
		}
	}

}
