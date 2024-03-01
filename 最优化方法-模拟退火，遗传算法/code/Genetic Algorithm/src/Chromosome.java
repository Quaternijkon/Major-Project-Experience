import java.util.*;

/**
 * @author DOCTORY
 */
public class Chromosome {

    /**
     * 生成一条新的染色体
     */
    public Chromosome() {
        this.geneData = new int[geneLength];
    }

    /**
     * 随机生成染色体片段, 计算初始适应值
     */
    public void initChromosome() {
        initGeneData();
        calculateRawFitness();
    }

    private static int geneLength;
    private static int geneDepth;

    private int rawFitness;

    private int calibratedFitness;

    public int[] geneData;

    static Random random = new Random();

    /**
     * 计算初始适应值
     */
    public void calculateRawFitness() {
        int[][] dp     = new int[geneLength][geneDepth];
        int[][] values = Main.timeTab;

        dp[0][0] = values[geneData[0]][0];

        for (int i = 1; i < geneLength; i++) {
            dp[i][0] = dp[i - 1][0] + values[geneData[i]][0];
        }
        for (int j = 1; j < geneDepth; j++) {
            dp[0][j] = dp[0][j - 1] + values[geneData[0]][j];
        }
        for (int i = 1; i < geneLength; i++) {
            for (int j = 1; j < geneDepth; j++) {
                dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]) + values[geneData[i]][j];
            }
        }
        rawFitness = dp[geneLength - 1][geneDepth - 1];
    }


    /**
     * 此内部类提供交叉交换的方法
     */
    static class Crossover {

        public static void crossover(Chromosome parent1, Chromosome parent2) {
            cycleCrossover(parent1, parent2);
        }

        /**
         * 循环交叉法生成子代,子代的基因的位置与父代相同
         */
        public static void cycleCrossover(Chromosome parent1, Chromosome parent2) {
            /* KEY: value, VALUE: index. */
            HashMap<Integer, Integer> map1 = new HashMap<>(geneLength);
            for (int i = 0; i < geneLength; i++) {
                map1.put(parent1.geneData[i], i);
            }
            Chromosome child1   = new Chromosome();
            Chromosome child2   = new Chromosome();
            boolean[]  visited  = new boolean[geneLength];
            boolean    onesTurn = true;
            /* 父代染色体交叉交换 */
            for (int i = 0; i < geneLength; i++) {
                if (visited[i]) {
                    continue;
                }
                int j = i;
                do {
                    visited[j] = true;
                    if (onesTurn) {
                        child1.geneData[j] = parent1.geneData[j];
                        child2.geneData[j] = parent2.geneData[j];
                    } else {
                        child1.geneData[j] = parent2.geneData[j];
                        child2.geneData[j] = parent1.geneData[j];
                    }
                    j = map1.get(parent2.geneData[j]);
                } while (j != i);
                onesTurn = !onesTurn;
            }
            /* 子代染色体以一定几率发生变异 */
            Random random = new Random();
            if (random.nextDouble() < 0.1) {
                child1.swap(random.nextInt(geneLength), random.nextInt(geneLength));
                child2.swap(random.nextInt(geneLength), random.nextInt(geneLength));
            }

            child1.calculateRawFitness();

            child2.calculateRawFitness();

            Population.chromosomes.add(child1);
            Population.chromosomes.add(child2);

        }

        public static void orderCrossover(Chromosome parent1, Chromosome parent2) {
        /*
         染色体交换片段
         Order Crossover (OX)
         若不控制随机数的值,有 p= 2/(geneLength)^2的概率不发生交换, 即完全保留亲本.
        */
            int     rand1, rand2;
            boolean isNotValidExchange;
            do {
                rand1 = random.nextInt(geneLength);
                rand2 = random.nextInt(geneLength - rand1) + rand1;
                isNotValidExchange = (rand1 == 0 && rand2 == geneLength) || (rand1 == rand2 && parent1.geneData[rand1] == parent2.geneData[rand2]);
            } while (isNotValidExchange);

            Chromosome child1 = new Chromosome();
            Chromosome child2 = new Chromosome();

            boolean[] visited1 = new boolean[geneLength];
            boolean[] visited2 = new boolean[geneLength];

            for (int i = rand1; i <= rand2; i++) {
                child1.geneData[i] = parent1.geneData[i];
                visited1[parent1.geneData[i]] = true;
                child2.geneData[i] = parent2.geneData[i];
                visited2[parent2.geneData[i]] = true;
            }

            int target1 = 0, target2 = 0;

            for (int i = 0; i < geneLength; i++) {
                while (target1 >= rand1 && target1 <= rand2) {
                    target1++;
                }
                while (target2 >= rand1 && target2 <= rand2) {
                    target2++;
                }
                if (!visited1[parent2.geneData[i]]) {
                    child1.geneData[target1] = parent2.geneData[i];
                    target1++;
                }
                if (!visited2[parent1.geneData[i]]) {
                    child2.geneData[target2] = parent1.geneData[i];
                    target2++;
                }
            }
            /* 考虑变异 */

            child1.calculateRawFitness();
            child2.calculateRawFitness();

            Population.chromosomes.add(child1);
            Population.chromosomes.add(child2);

        }

        public static void microbialCrossover(Chromosome parent1, Chromosome parent2) {/* p1适应值高, p2适应值低 */
            HashMap<Integer, Integer> map1 = new HashMap<>(geneLength);
            for (int i = 0; i < geneLength; i++) {
                map1.put(parent1.geneData[i], i);
            }
            boolean[] visited = new boolean[geneLength];

            int index = 0;
            do {
                visited[parent1.geneData[index]] = true;
                parent2.geneData[index] = parent1.geneData[index];

                index = map1.get(parent2.geneData[index]);
            } while (index != 0);

            int target=0;
            for (int i = 0; i < geneLength; i++) {
                if (!visited[parent2.geneData[i]]) {
                    parent2.geneData[target] = parent2.geneData[i];
                    target++;
                }
            }

            /* 考虑变异 */

            parent1.calculateRawFitness();
            parent2.calculateRawFitness();
            Population.chromosomes.add(parent1);
            Population.chromosomes.add(parent2);

        }
    }

    /**
     * 此内部类提供变异的方法
     */
    class Mutate {

        public void mutate() {

        }

        static Random rand = new Random();

        /**
         * 染色体随机交换片段
         */
        public void ectopicMutate() {
            final double ECTOPIC_POSSIBILITY = 0.5;
            do {
                int random1 = rand.nextInt(geneLength);
                int random2 = rand.nextInt(geneLength);
                swap(random1, random2);
            } while (rand.nextDouble() < ECTOPIC_POSSIBILITY);
        }

        /**
         * 染色体倒转
         */
        public void reversedMutate() {
            for (int i = 0; i < geneLength / 2; i++) {
                swap(i, geneLength - 1 - i);
            }
        }

        /**
         * 剪切染色体后重新拼接
         */
        public void shearMutate() {
            int cutPoint;
            do {
                cutPoint = rand.nextInt(geneLength);
            } while (cutPoint == 0);
            int[] geneDataSaved = geneData;
            geneData = new int[geneLength];
            int index = 0;
            for (int i = cutPoint; i < geneLength; i++) {
                geneData[index++] = geneDataSaved[i];
            }
            for (int i = 0; i < cutPoint; i++) {
                geneData[index++] = geneDataSaved[i];
            }

        }

        /**
         * 随机重置染色体上的所有基因
         */
        public void resetMutate() {
            for (int i = geneLength; i > 0; i--) {
                swap(random.nextInt(i), i - 1);
            }
        }


    }


    /**
     * 打印染色体信息
     */
    public static void show(List<Chromosome> chromosomeList) {
        for (Chromosome ch : chromosomeList) {
            int     m = Main.Row_Workpiece, n = Main.Column_Machine;
            int[][] t = new int[m][n], a = Main.timeTab;
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    t[i][j] = a[ch.geneData[i]][j];
                }
            }
            int[][] dp = new int[m][n];
            dp[0][0] = t[0][0];
            /* 因为第一行的格子只能从第一行中此前的格子以"向右"的方法到达,第一列同理.避免循环中额外的判断,将第一行和第一列单独计算. */
            for (int i = 1; i < m; i++) {
                dp[i][0] = dp[i - 1][0] + t[i][0];
            }
            for (int i = 1; i < n; i++) {
                dp[0][i] = dp[0][i - 1] + t[0][i];
            }
            for (int i = 1; i < m; i++) {
                for (int j = 1; j < n; j++) {
                    dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]) + t[i][j];
                }
            }
            /* 时间表右下角的值即为当前加工顺序下的最小工作时间 */
            int v = dp[m - 1][n - 1];
            if (v < Population.min) {
                Population.min = v;

            }
//            System.out.println(" fitness: " + ch.calibratedFitness + " geneData: " + Arrays.toString(ch.geneData) + " value: " + v);
        }
        System.out.println(Population.min);
    }

    public void swap(int i, int j) {
        /* 交换 */
        int temp = geneData[i];
        geneData[i] = geneData[j];
        geneData[j] = temp;
    }

    /**
     * 随机初始化染色体基因序列
     */
    public void initGeneData() {
        for (int i = 0; i < geneLength; i++) {
            geneData[i] = i;
        }
        for (int i = geneLength; i > 0; i--) {
            swap(random.nextInt(i), i - 1);
        }
    }

    public static void setGeneLength(int geneLength) {
        Chromosome.geneLength = geneLength;
    }

    public static void setGeneDepth(int geneDepth) {
        Chromosome.geneDepth = geneDepth;
    }

    public int getCalibratedFitness() {
        return calibratedFitness;
    }

    public int getRawFitness() {
        return rawFitness;
    }

    public void setCalibratedFitness(int calibratedFitness) {
        this.calibratedFitness = calibratedFitness;
    }

}
