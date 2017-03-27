import java.util.*;
import java.util.concurrent.*;
import util.Util;


public class ExecutorService_MatrixMultiplication
{
   public static void main (String arg[]) throws Exception
   {
      int dataA[][] = new int[][]
      {
         {1, 4},
         {2, 5},
         {3, 6},
        
      };
      
      int dataB[][] = new int[][]
      {
         {1,  3, 5},
         {2,  4, 6}
      };
      
      ExecutorService executor = Executors.newFixedThreadPool(3);
      
      Matrix matrixA = new Matrix (dataA);
      Matrix matrixB = new Matrix (dataB);
      System.out.println(matrixA.multiply(matrixB));
      
      executor.shutdown();
   }
   
   static class Matrix
   {
      private static final int THREAD_COUNT = 3;
      
      private int rowCount, colCount, data[][];
      
      public Matrix (int data[][])
      {
         this.data = data;
         this.rowCount = data.length;
         this.colCount = data[0].length;
      }
      
   public Matrix multiply (Matrix m)
   {
      ExecutorService executor = Executors.newFixedThreadPool(THREAD_COUNT);
      
      try
      {
         if (this.colCount != m.rowCount)
            throw new IllegalArgumentException ("Matrix col count does not match argument's row count. ColCount=" + this.colCount + " RowCount=" + m.rowCount);
         
         List<Task> listFrag = Matrix.getTasks(this, m);
         List<Future<Integer>> listResult = executor.invokeAll(listFrag);
         
         int data[][] = new int [this.rowCount][m.colCount];
         for (int i = 0; i < this.rowCount; ++i)
            for (int j = 0; j < m.colCount; ++j)
               data[i][j] = listResult.get(i*m.colCount + j).get();
         
         return new Matrix (data);
      }
      catch (Exception e)
      {
         executor.shutdown();
      }
      return null;
   }
      
      public static List<Task> getTasks (Matrix matrixA, Matrix matrixB)
      {
         List<Task> listFrag = new ArrayList<> ();
         for (int i = 0; i < matrixA.rowCount; ++i)
            for (int j = 0; j < matrixB.colCount; ++j)
               listFrag.add(new Task (matrixA, matrixB, i, j));
         return listFrag;
      }
      
      public String toString ()
      {
         StringBuilder builder = new StringBuilder ();
         for (int i = 0; i < rowCount; ++i)
         {
            for (int j = 0; j < colCount; ++j)
               builder.append(String.format(" %3d ", data[i][j]));
            builder.append(Util.lineSep);//adauga
         };
         return builder.toString();
      }
   }
      
   static class Task implements Callable<Integer>
   {
      int rowId, colId;
      
      Matrix matrixA, matrixB;
      
      public Task (Matrix matrixA, Matrix matrixB, int rowId, int colId)
      {
         this.rowId = rowId;
         this.colId = colId;
         this.matrixA = matrixA;
         this.matrixB = matrixB;
      }
   
      @Override
      public Integer call() throws Exception
      {
         String mesg = String.format("A[%d] * B[%d]", rowId, colId);
         Util.threadLog("Started  Task. " + mesg);
         int product = 0;
         for (int i = 0; i < matrixA.colCount; ++i)
            product = product + matrixA.data[rowId][i] * matrixB.data[i][colId];
         Util.threadLog("Finished Task. " + mesg + " = " + product);
         return product;
      }
      
   }
}