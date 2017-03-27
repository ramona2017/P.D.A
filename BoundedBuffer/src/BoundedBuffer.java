
   import java.util.Date;
   import java.util.concurrent.Semaphore;


    interface Buffer{

       public abstract void insert(Object item);
   
 
       public abstract Object remove();
   }

class BoundedBuffer implements Buffer{ 
    
      private static final int BUFFER_SIZE = 3; 
      private int count; 
      private int in;   
      private int out;  
      private Object[] buffer;
      private Semaphore mutex; //provides limited access to the buffer (mutual exclusion)
      private Semaphore empty;
      private Semaphore full; 
       
       public BoundedBuffer(){
     
         count = 0;
         in = 0;
         out = 0;
         buffer = new Object[BUFFER_SIZE];
         mutex = new Semaphore(1); 
         empty = new Semaphore(BUFFER_SIZE); //array begins with all empty elements
         full = new Semaphore(0); //array begins with no elements
      }
   

       public void insert(Object item) {
         try{
            empty.acquire(); 
            
            mutex.acquire(); //mutual exclusion
         }
             catch (InterruptedException e) { 
               System.out.println("ERROR in insert(): " + e);
            } 
      	
      // add an item to the buffer
         ++count;
         buffer[in] = item;
         in = (in + 1) % BUFFER_SIZE; 
         
    
         if (count == BUFFER_SIZE){
            System.out.println("BUFFER FULL "
               + "Producer inserted \"" + item 
               + "\" count=" + count + ", "
               + "in=" + in + ", out=" + out);
         }
         else{
            System.out.println("Producer inserted \"" + item 
               + "\" count=" + count + ", "
               + "in=" + in + ", out=" + out);
         }
       
         mutex.release(); 
         full.release(); //keep track of number of elements (value++)      	
         //If buffer was empty, then this wakes up the Consumer
      }
  
       public Object remove() {
         Object item=null;
   
         try{
            full.acquire(); 
            

            mutex.acquire(); 
         }
             catch (InterruptedException e) { 
               System.out.println("ERROR in try(): " + e);
            } 
      	
      // remove an item from the buffer
         --count;
         item = buffer[out]; 
         out = (out + 1) % BUFFER_SIZE;
      
     
         if (count == 0){
            System.out.println("BUFFER EMPTY "
               + "Consumer removed \"" + item 
               + "\" count=" + count + ", "
               + "in=" + in + ", out=" + out);
         }
         else{
            System.out.println("Consumer removed \"" + item 
               + "\" count=" + count + ", "
               + "in=" + in + ", out=" + out);
         }
       
         mutex.release(); //mutual exclusion
         empty.release(); //keep track of number of empty elements (value++)      	  
      	//if buffer was full, then this wakes up the Producer 
         return item;
      }
   
   }



    class Producer implements Runnable{
    
      private  Buffer buffer;
             
       public Producer(Buffer b) {
         buffer = b;
      }
   
       public void run(){
         Date message;
         while (true) {
            System.out.println("Producer napping");
            SleepUtilities.nap();
         // produce an item & enter it into the buffer
            message = new Date();      
            System.out.println("Producer produced \"" + message + "\"");
            buffer.insert(message);
         }
      }
   }

//*******************************************************

/**
 * This is the consumer thread for the bounded buffer problem.
 */

    class Consumer implements Runnable{
    
      private Buffer buffer;
       
       public Consumer(Buffer b) { 
         buffer = b;
      }
   
       public void run(){
         Date message = null;
         while (true){
            System.out.println("Consumer napping");
            SleepUtilities.nap(); 
         
            System.out.println("Consumer wants to consume");
            message = (Date)buffer.remove();
            System.out.println("Consumer consumed \"" + message + "\"");
         }
      }
   }

//*********************************************************

/**
 * Utilities for causing a thread to sleep.
 * Note, we should be handling interrupted exceptions
 * but choose not to do so for code clarity.
 *
 */

    class SleepUtilities{
    
      private static final int NAP_TIME = 5; //max nap time in seconds
    
   /**
    * Nap between zero and NAP_TIME seconds.
    */
       public static void nap() {
         nap(NAP_TIME);
      }
   
   /**
    * Nap between zero and duration seconds.
    */
       public static void nap(int duration) {
         int sleeptime = (int) (NAP_TIME * Math.random() );
         System.out.println("Nap for " + sleeptime + " seconds"); 
         //Causes the currently executing thread to sleep (cease execution) 
      	//for the specified number of milliseconds, 
      	//subject to the precision and accuracy of system timers and schedulers.
         try { Thread.sleep(sleeptime*1000); }
             catch (InterruptedException e) {
             //method sleep() throws InterruptedException - if any thread has interrupted the current thread. 
               System.out.println("ERROR in nap(): " + e);
            }
      }
   }