  import java.util.Date;
   import java.util.concurrent.Semaphore;
 
    public class Solution{
       public static void main(String args[]) {
   
         Buffer sharedBuffer = new BoundedBuffer();
    
         Thread producerThread = new Thread(new Producer(sharedBuffer));
         Thread consumerThread = new Thread(new Consumer(sharedBuffer));
      		
         producerThread.start();
         consumerThread.start();               
      }
   }