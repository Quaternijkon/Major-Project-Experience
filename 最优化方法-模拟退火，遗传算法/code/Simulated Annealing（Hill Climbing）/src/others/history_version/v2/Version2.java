package others.history_version.v2;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Random;
import java.util.Scanner;

public class Version2 {
    //模拟退火的参数设置
    static final int REPETITIONS_SAME_TEMPERATURE = 206, REPETITIONS_SAME_SAMPLE = 1;
    static final double INITIAL_TEMPERATURE = 1500.0, CRITICAL_TEMPERATURE = 1e-4, SA_RATIO = 0.99;

    static int Row_Workpiece, Column_Machine;//工件数(行), 机器数(列)
    static int currentTime, nextTime;//当前状态所需的时间, 做出改变后所需的时间
    static int workpieceExchange1, workpieceExchange2;//随机交换工件的索引
    static double T;
    static int garbage;
    static File fileInput = new File("C:\\Users\\DOCTORY\\Desktop\\SA.txt");
    static File fileOutput = new File("C:\\Users\\DOCTORY\\Desktop\\ANS.txt");

    static Random rand = new Random();

    public static void main(String[] args) {
        System.out.println("Begin");
        //重复运行REPETITIONS_SAME_SAMPLE次
        for (int order = 1; order <= REPETITIONS_SAME_SAMPLE; order++) {
            T = INITIAL_TEMPERATURE;

            long start = System.currentTimeMillis();

            try (Scanner scanner = new Scanner(fileInput)) {

                //初始化
                Row_Workpiece = scanner.nextInt();
                Column_Machine = scanner.nextInt();
                int[][] current = new int[Row_Workpiece][Column_Machine];
                int[][] next = new int[Row_Workpiece][Column_Machine];

                for (int i = 0; i < Row_Workpiece; i++) {
                    for (int j = 0; j < Column_Machine; j++) {
                        garbage = scanner.nextInt();
                        current[i][j] = next[i][j] = scanner.nextInt();
                    }
                }

                if (scanner.hasNext()) {
                    System.out.println("ERROR" + scanner.nextInt());
                } else {
                    System.out.println("SUCCEED");
                }

                //模拟退火算法
                simulatedAnnealing(current, next);

//                PrintWriter printWriter = new PrintWriter(fileOutput);
//                for (int i = 0; i < Row_Workpiece; i++){
//                    printWriter.println();
//                    for (int j = 0; j < Column_Machine; j++)
//                        printWriter.print(current[i][j] + " ");
//                }
//                printWriter.close();


            } catch (FileNotFoundException e) {
                e.printStackTrace();
                System.out.println("ERROR");
            }

            long end = System.currentTimeMillis();

            //输出结果
            printResult(order, start, end);


        }

    }

    private static void printResult(int order, long start, long end) {
        //计算消耗的时间
        long totalMillisecond = end - start;
        long currentMillisecond = totalMillisecond % 1000;

        long totalSecond = totalMillisecond / 1000;
        long currentSecond = totalSecond % 60;

        long totalMinutes = totalSecond / 60;
        long currentMinute = totalMinutes % 60;

        System.out.println("NO." + order + " result:" + currentTime);
        System.out.println("Time:" + currentMinute + ":" + currentSecond + ":" + currentMillisecond);

    }

    private static void simulatedAnnealing(int[][] current, int[][] next) {
        int delta;

        currentTime = getMinTimeCost(current);
        //退火算法的核心
        while (T > CRITICAL_TEMPERATURE) {
            for (int num = 0; num < REPETITIONS_SAME_TEMPERATURE; num++) {

                randomExchange(next);//随机交换工件加工顺序

                nextTime = getMinTimeCost(next);//获取做出改变后消耗的时间

                delta = nextTime - currentTime;

                if (delta < 0) {
                    currentTime = nextTime;

                    exchange(current);//认为这种改变是有帮助的

                } else {
                    double r = rand.nextDouble();
                    if (r < Math.exp(-delta / T)) {
                        currentTime = nextTime;

                        exchange(current);//认为这种改变是可以尝试的

                    } else {
                        exchange(next);//改变不可取, 恢复原样.
                    }
                }
            }
            T *= SA_RATIO;
        }
    }

    private static int getMinTimeCost(final int[][] value) {
        //使用动态规划算法求解特定加工顺序下所需的最短时间, 即在矩阵中找到一条从左上角以"向下"和"向右"的动作到达右下角, 求和最大的一条路线.
        int[][] dp = new int[Row_Workpiece][Column_Machine];
        dp[0][0] = value[0][0];
        //任意一格的值等于dp矩阵中其左边和上边的值的最大值+加工时间矩阵上其值,即dp[i][j]=max(left,up)+value[i][j]
        for (int i = 1; i < Row_Workpiece; i++) {
            dp[i][0] = dp[i - 1][0] + value[i][0];
        }
        for (int i = 1; i < Column_Machine; i++) {
            dp[0][i] = dp[0][i - 1] + value[0][i];
        }
        for (int i = 1; i < Row_Workpiece; i++) {
            for (int j = 1; j < Column_Machine; j++) {
                dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]) + value[i][j];
            }
        }
        return dp[Row_Workpiece - 1][Column_Machine - 1];
    }

    private static void randomExchange(int[][] next) {
        do {
            workpieceExchange1 = rand.nextInt(Row_Workpiece);
            workpieceExchange2 = rand.nextInt(Row_Workpiece);
        } while (workpieceExchange1 == workpieceExchange2);
        exchange(next);
    }

    private static void exchange(int[][] next) {
        int temp;
        for (int i = 0; i < Column_Machine; i++) {
            temp = next[workpieceExchange1][i];
            next[workpieceExchange1][i] = next[workpieceExchange2][i];
            next[workpieceExchange2][i] = temp;
        }
    }

}