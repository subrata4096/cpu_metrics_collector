
import java.lang.instrument.Instrumentation;

public class ProfilerMain {
public static void premain(String agentArguments,
Instrumentation instrumentation) {
final ProfilerTransformer pt = new ProfilerTransformer();

/*
Runtime.getRuntime().addShutdownHook(new Thread() {
public void run() {
pt.dumpMetrics();
}
});
*/

instrumentation.addTransformer(pt);
}

//pt.attachShutDownHook();

}
