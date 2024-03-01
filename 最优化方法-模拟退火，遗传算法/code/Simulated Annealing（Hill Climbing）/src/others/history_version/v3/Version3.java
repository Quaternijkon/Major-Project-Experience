//package others.history_version.v3;
//
//import java.io.File;
//import java.io.FileNotFoundException;
//import java.io.PrintWriter;
//import java.others.util.*;
//
///**
// * @author DONG RUOYANG
// */
//public class Version3 {
//
//    static final int AVAILABLE_PROCESSORS = Runtime.getRuntime().availableProcessors();
//    static final int REPETITIONS_SAME_TEMPERATURE = 206, REPETITIONS_SAME_SAMPLE = 8 * AVAILABLE_PROCESSORS;
//    static final double INITIAL_TEMPERATURE = 1500.0, CRITICAL_TEMPERATURE = 1e-4, SA_RATIO = 0.99;
//
//    static int Row_Workpiece, Column_Machine;
//
//    static final File FILE_INPUT = new File("C:\\Users\\DOCTORY\\Desktop\\SA9.txt");
//    static final File FILE_OUTPUT = new File("C:\\Users\\DOCTORY\\Desktop\\ANS9.txt");
//
//    static Random rand = new Random();
//    static PrintWriter printWriter;
//
//    static {
//        try {
//            printWriter = new PrintWriter(FILE_OUTPUT);
//        } catch (FileNotFoundException e) {
//            e.printStackTrace();
//        }
//    }
//
//    static int[][] data;
//
//    static final List<Thread> THREADS = new ArrayList<>();
//    static final List<Integer> RESULTS = new ArrayList<>();
//
//    public static void main(String[] args) {
//        System.out.println("Begin");
//
//        long start = System.currentTimeMillis();
//
//        try (Scanner scanner = new Scanner(FILE_INPUT)) {
//            Row_Workpiece = scanner.nextInt();
//            Column_Machine = scanner.nextInt();
//            data = new int[Row_Workpiece][Column_Machine];
//
//            for (int i = 0; i < Row_Workpiece; i++) {
//                for (int j = 0; j < Column_Machine; j++) {
//                    scanner.nextInt();
//                    data[i][j] = scanner.nextInt();
//                }
//            }
//
//            if (scanner.hasNext()) {
//                System.out.println("ERROR" + scanner.nextInt());
//            } else {
//                System.out.println("SUCCEED");
//            }
//
//        } catch (FileNotFoundException e) {
//            e.printStackTrace();
//        }
//
//        for (int k = 0; k < REPETITIONS_SAME_SAMPLE; k++) {
//            Thread thread = new Thread(new IndividualTask());
//            thread.start();
//            THREADS.add(thread);
//            if (THREADS.size() % AVAILABLE_PROCESSORS == 0) {
//                waitForThreads();
//            }
//        }
//
//        long end = System.currentTimeMillis();
//
//        printResult(start, end);
//
//    }
//
//    private static void waitForThreads() {
//        for (Thread thread : Version3.THREADS) {
//
//            try {
//                thread.join();
//            } catch (InterruptedException e) {
//                e.printStackTrace();
//            }
//        }
//        Version3.THREADS.clear();
//    }
//
//    private static void printResult(long start, long end) {
//
//        long totalMillisecond = end - start;
//        long currentMillisecond = totalMillisecond % 1000;
//
//        long totalSecond = totalMillisecond / 1000;
//        long currentSecond = totalSecond % 60;
//
//        long totalMinutes = totalSecond / 60;
//        long currentMinute = totalMinutes % 60;
//
//        System.out.println("Time:" + currentMinute + ":" + currentSecond + ":" + currentMillisecond);
//        printWriter.println("Time:" + currentMinute + ":" + currentSecond + ":" + currentMillisecond);
//
//        RESULTS.sort(Comparator.naturalOrder());
//        RESULTS.forEach(printWriter::println);
//
//        printWriter.close();
//
//    }
//
//
//    static class IndividualTask implements Runnable {
//        int currentTime, nextTime;
//        int workpieceExchange1, workpieceExchange2;
//        double t = INITIAL_TEMPERATURE;
//
//        private final int[][] current = new int[Row_Workpiece][Column_Machine];
//        private final int[][] next = new int[Row_Workpiece][Column_Machine];
//
//        public IndividualTask() {
//            for (int i = 0; i < Row_Workpiece; i++) {
//                for (int j = 0; j < Column_Machine; j++) {
//                    current[i][j] = next[i][j] = data[i][j];
//                }
//            }
//        }
//
//        @Override
//        public void run() {
//            simulatedAnnealing(current, next);
//            RESULTS.add(currentTime);
//            System.out.println(Thread.currentThread().getName() + " " + currentTime);
//        }
//
//
//        private void simulatedAnnealing(int[][] current, int[][] next) {
//            int delta;
//
//            currentTime = getMinTimeCost(current);
//
//            while (t > CRITICAL_TEMPERATURE) {
//                for (int num = 0; num < REPETITIONS_SAME_TEMPERATURE; num++) {
//
//                    randomExchange(next);
//
//                    nextTime = getMinTimeCost(next);
//
//                    delta = nextTime - currentTime;
//
//                    if (delta < 0) {
//                        currentTime = nextTime;
//
//                        exchange(current);
//
//                    } else {
//                        double r = rand.nextDouble();
//                        if (r < Math.exp(-delta / t)) {
//                            currentTime = nextTime;
//
//                            exchange(current);
//
//                        } else {
//                            exchange(next);
//                        }
//                    }
//                }
//                t *= SA_RATIO;
//            }
//        }
//
//        private int getMinTimeCost(final int[][] value) {
//
//            int[][] dp = new int[Row_Workpiece][Column_Machine];
//            dp[0][0] = value[0][0];
//
//            for (int i = 1; i < Row_Workpiece; i++) {
//                dp[i][0] = dp[i - 1][0] + value[i][0];
//            }
//            for (int i = 1; i < Column_Machine; i++) {
//                dp[0][i] = dp[0][i - 1] + value[0][i];
//            }
//            for (int i = 1; i < Row_Workpiece; i++) {
//                for (int j = 1; j < Column_Machine; j++) {
//                    dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]) + value[i][j];
//                }
//            }
//            return dp[Row_Workpiece - 1][Column_Machine - 1];
//        }
//
//        private void randomExchange(int[][] next) {
//            do {
//                workpieceExchange1 = rand.nextInt(Row_Workpiece);
//                workpieceExchange2 = rand.nextInt(Row_Workpiece);
//            } while (workpieceExchange1 == workpieceExchange2);
//            exchange(next);
//        }
//
//        private void exchange(int[][] next) {
//            int temp;
//            for (int i = 0; i < Column_Machine; i++) {
//                temp = next[workpieceExchange1][i];
//                next[workpieceExchange1][i] = next[workpieceExchange2][i];
//                next[workpieceExchange2][i] = temp;
//            }
//        }
//    }
//}
//
