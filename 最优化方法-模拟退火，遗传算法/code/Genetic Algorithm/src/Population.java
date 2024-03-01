import java.util.ArrayList;
import java.util.List;
import java.util.Random;

/**
 * @author DOCTORY
 */
public class Population {

    public static final int    NUMBER_OF_SPECIES = 40;
    public static final double ITERATION         = 0.99;
    public static final double EPSILON           = 0;
    public static       double epsilon           = EPSILON;

    public static int min=9999999;


    public static int maxFitnessValue;
    public static int calibratedFitnessSum;

    public static int[] turntable = new int[NUMBER_OF_SPECIES];

    static List<Chromosome> chromosomes      = new ArrayList<>();
    static List<Chromosome> chromosomesSaved = new ArrayList<>();

    public static int parentIndex1, parentIndex2;

    static Random random = new Random();

    /**
     * 找到最大的初始值
     */
    public static void findMaxFitnessValue(List<Chromosome> chromosomeList) {
        for (Chromosome ch : chromosomeList) {
            if (ch.getRawFitness() > maxFitnessValue) {
                maxFitnessValue = ch.getRawFitness();
            }
        }
    }

    /**
     * 标定所有染色体的适应值
     */
    public static void calibrateFitness(List<Chromosome> chromosomeList) {
        for (Chromosome ch : chromosomeList) {
            ch.setCalibratedFitness(maxFitnessValue - ch.getRawFitness() + (int) epsilon);
        }
    }

    /**
     * 计算标定适应值之和
     */
    public static void calculateCalibratedFitnessValueSum(List<Chromosome> chromosomeList) {
        calibratedFitnessSum = 0;
        for (Chromosome ch : chromosomeList) {
            calibratedFitnessSum += ch.getCalibratedFitness();
        }
    }

    /**
     * 根据标定适应值生成转盘, 完成了1.寻找最大初始适应值;2.标定适应值;3.生成转盘;4.计算标定适应值之和.
     */
    public static void generateTurntable(List<Chromosome> chromosomeList) {
        findMaxFitnessValue(chromosomeList);
        calibrateFitness(chromosomeList);
        int value = 0;
        for (int i = 0; i < chromosomeList.size(); i++) {
            value += chromosomeList.get(i).getCalibratedFitness();
            turntable[i] = value;
        }
        calibratedFitnessSum = value;
    }

    /**
     * 使用转盘正比例选择双亲
     */
    public static void selectParents() {
        int rand1 = random.nextInt(calibratedFitnessSum);
        int rand2 = random.nextInt(calibratedFitnessSum);
        parentIndex1 = 0;
        parentIndex2 = 0;

        /* 查找双亲 */
        boolean parent1Found = false;
        boolean parent2Found = false;
        while (!(parent1Found && parent2Found)) {
            if (!parent1Found && turntable[parentIndex1] < rand1) {
                parentIndex1++;
            } else {
                parent1Found = true;
            }
            if (!parent2Found && turntable[parentIndex2] < rand2) {
                parentIndex2++;
            } else {
                parent2Found = true;
            }
        }

    }

    public static void iterateEpsilon() {
        epsilon *= ITERATION;
    }

    public static void setEpsilonMaxFitness() {
        epsilon = 1000;
    }
}
