package others.util;

import java.io.*;
import java.util.Random;
import java.util.Scanner;

public class RandomInstanceProducer {
    static final File FILE_OUTPUT = new File("C:\\Users\\DOCTORY\\Desktop\\SA.txt");
    static Scanner scanner = new Scanner(System.in);
    static Random rand = new Random();
    public static void main(String[] args) {
        int row,column;
        int upLimit,lowLimit;
        System.out.println("Please Input The Row And Column Number Of The Instance.");
        row=scanner.nextInt();
        column=scanner.nextInt();
        System.out.println("Please Input The Lower And Upper Limits.");
        lowLimit=scanner.nextInt();
        upLimit=scanner.nextInt();
        try(PrintWriter printlnWriter=new PrintWriter(FILE_OUTPUT)) {
            printlnWriter.println(row+" "+column);
            for (int i=0; i<row; i++) {
                for (int j=0; j<column; j++) {
                    printlnWriter.print(j+" "+(rand.nextInt(upLimit-lowLimit)+lowLimit)+" ");
                }
                printlnWriter.println();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println("Done!");
    }
}
