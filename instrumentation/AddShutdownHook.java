public class AddShutdownHook {
 public void attachShutDownHook(){
  Runtime.getRuntime().addShutdownHook(new Thread() {
   @Override
   public void run() {
    System.out.println("Inside Add Shutdown Hook");
   }
  });
  System.out.println("Shut Down Hook Attached.");
 }
}
