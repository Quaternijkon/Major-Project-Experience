//package others.history_version.v1;
//
//import java.others.util.Random;
//import java.others.util.Scanner;
//
//public class Version1 {
//    static int Row_Workpiece, Column_Machine;
//    static int currentTime, nextTime;
//    static double T = 50000.0;
//
//    static Random rand = new Random();
//    static Scanner scanner = new Scanner(System.in);
//
//    public static void main(String[] args) {
//        Row_Workpiece = scanner.nextInt();
//        Column_Machine = scanner.nextInt();
//        int[][] current = new int[Row_Workpiece][Column_Machine];
//        int[][] next = new int[Row_Workpiece][Column_Machine];
//        for (int i = 0; i < Row_Workpiece; i++) {
//            for (int j = 0; j < Column_Machine; j++) {
//                current[i][j] = next[i][j] = scanner.nextInt();
//            }
//        }
//
//        simulatedAnnealing(current, next);
//
//        System.out.println(currentTime);
////        for (int i = 0; i < Row_Workpiece; i++)
////            for (int j = 0; j < Column_Machine; j++)
////                System.out.print(current[i][j] + " ");
//    }
//
//    private static void simulatedAnnealing(int[][] current, int[][] next) {
//        int delta;
//        while (T > 1e-9) {
//            for (int num = 0; num < 5000; num++) {
//
//                currentTime = getMinTimeCost(current);
//
//                randomExchange(next);
//
//                nextTime = getMinTimeCost(next);
//
//                delta = nextTime - currentTime;
//
//                if (delta <= 0) {
//                    currentTime = nextTime;
//                    arrayCopyTo(next, current);
//
//                } else {
//                    double r = rand.nextDouble();
//                    if (r < Math.exp(-delta / T)) {
//                        currentTime = nextTime;
//                        arrayCopyTo(next, current);
//
//                    } else {
//                        arrayCopyTo(current, next);
//                    }
//                }
//            }
//            T *= 0.99;
//        }
//    }
//
//    private static int getMinTimeCost(final int[][] value) {
//        int[][] dp = new int[Row_Workpiece][Column_Machine];
//        dp[0][0] = value[0][0];
//        for (int i = 1; i < Row_Workpiece; i++) {
//            dp[i][0] = dp[i - 1][0] + value[i][0];
//        }
//        for (int i = 1; i < Column_Machine; i++) {
//            dp[0][i] = dp[0][i - 1] + value[0][i];
//        }
//        for (int i = 1; i < Row_Workpiece; i++) {
//            for (int j = 1; j < Column_Machine; j++) {
//                dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]) + value[i][j];
//            }
//        }
//        return dp[Row_Workpiece - 1][Column_Machine - 1];
//    }
//
//    private static void randomExchange(int[][] next) {
//        int workpieceExchange1 = rand.nextInt(Row_Workpiece);
//        int workpieceExchange2 = rand.nextInt(Row_Workpiece);
//        int temp;
//        for (int i = 0; i < Column_Machine; i++) {
//            temp = next[workpieceExchange1][i];
//            next[workpieceExchange1][i] = next[workpieceExchange2][i];
//            next[workpieceExchange2][i] = temp;
//        }
//
//    }
//
//    private static void arrayCopyTo(int[][] next, int[][] current) {
//        for (int p = 0; p < Row_Workpiece; p++) {
//            System.arraycopy(next[p], 0, current[p], 0, Column_Machine);
//        }
//    }
//}