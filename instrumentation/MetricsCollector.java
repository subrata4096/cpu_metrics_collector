package com.ilaguna.metrics;

import java.util.ArrayList;
import java.util.GregorianCalendar;
import java.util.Calendar;
import java.io.*;

public class MetricsCollector {

	protected ArrayList<String> data;
	protected String methodName;
	protected File dir;
	protected BufferedWriter outFile;

	int[] metricsInStatFile = {10,12,14,15,20,23,24,28,30,39};

	public MetricsCollector(String method) {
		methodName = method;
		data = new ArrayList<String>();
		openOutputFile();
	}

	public void collectMetrics() {
		data.add(gatherData());
	}

	public void collectMetrics(String str) {
		data.add(str + "," + gatherData());
	}

	private String gatherData() {
		String data = null;
		data = getStatData() + getNumFileDescritors() + "," + getIOStats();
		return data;
	}


	public void printMetrics() {
		for (int i=0; i < data.size(); ++i)
			System.out.println(Integer.toString(i) + ":" + data.get(i));
	}

	private void openOutputFile() {
		try{
			dir = new File("./output");
			dir.mkdirs();
			Calendar cal = new GregorianCalendar();

			// Create file
			FileWriter fstream = new FileWriter("./output/" +
					Long.toString(cal.getTimeInMillis()) + "_" + PID + ".dat");
			outFile = new BufferedWriter(fstream);

			outFile.write(getMetricNames());
			outFile.write("\n");
		} catch (Exception e) { //Catch exception if any
			System.err.println("Error: " + e.getMessage());
			e.printStackTrace();
		}
	}

	public void saveData() {
		try{
			/*File dir = new File("./output");
			  dir.mkdirs();
			  Calendar cal = new GregorianCalendar();
			// Create file
			FileWriter fstream = new FileWriter("./output/" +
			Long.toString(cal.getTimeInMillis()) + "_" + PID + ".dat");
			BufferedWriter out = new BufferedWriter(fstream);
			out.write(getMetricNames());
			out.write("\n");*/
			for (int i=0; i < data.size(); ++i) {
				outFile.write(data.get(i));
				outFile.write("\n");
			}

			//Close the output stream
			outFile.close();
		} catch (Exception e) { //Catch exception if any
			System.err.println("Error: " + e.getMessage());
			e.printStackTrace();
		}
	}

	private String getMetricNames() {
		String ret = "ID,minflt,majflt,utime,stime,num_threads,vsize,rss," +
			"startstack,kstkeip,processor,num_file_desc,rchar,wchar," +
			"read_bytes,write_bytes,cancelled_write_bytes";
		return ret;
	}
}
