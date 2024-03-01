import java.util.*;

/**
 * @author DOCTORY
 */
public class Main {

    static int Row_Workpiece, Column_Machine;
    static int[][] timeTab;

    static final int GENERATION = 200;

    static final double POSSIBILITY_OF_REMAINING = 0.03, POSSIBILITY_OF_EXCHANGE = 0.91, POSSIBILITY_OF_VARIATION = 0.03, POSSIBILITY_OF_SELFING = 0.03;

    static Scanner scanner = new Scanner(System.in);

    public static void main(String[] args) {

        System.out.println("请输入工件数,机器数.");
        Row_Workpiece = scanner.nextInt();
        Column_Machine = scanner.nextInt();
        timeTab = new int[Row_Workpiece][Column_Machine];
        /* 初始化 */
        System.out.println("请输入各工件在各机器上消耗的时间.");
        for (int i = 0; i < Row_Workpiece; i++) {
            for (int j = 0; j < Column_Machine; j++) {
                scanner.nextInt();
                timeTab[i][j] = scanner.nextInt();
            }
        }
        Chromosome.setGeneLength(Row_Workpiece);
        Chromosome.setGeneDepth(Column_Machine);

        //创建初始随机染色体
        for (int i = 0; i < Population.NUMBER_OF_SPECIES; i++) {
            Chromosome individual = new Chromosome();
            individual.initChromosome();
            Population.chromosomes.add(individual);
        }

        Population.generateTurntable(Population.chromosomes);
        Chromosome.show(Population.chromosomes);

        System.out.println(Population.calibratedFitnessSum);

        Population.findMaxFitnessValue(Population.chromosomes);
        Population.setEpsilonMaxFitness();
        /* 开始演化 */
        for (int g = 0; g < GENERATION; g++) {
            /* 重置染色体数组 */
            Population.chromosomesSaved = Population.chromosomes;
            Population.chromosomes = new ArrayList<>();

            Population.generateTurntable(Population.chromosomesSaved);
            for (int i = 0; i < Population.NUMBER_OF_SPECIES / 2; i++) {
                Population.selectParents();
                Chromosome.Crossover.crossover(
                        Population.chromosomesSaved.get(Population.parentIndex1),
                        Population.chromosomesSaved.get(Population.parentIndex1));
            }

            Chromosome.show(Population.chromosomes);
            Population.iterateEpsilon();
        }

        Population.generateTurntable(Population.chromosomes);
//        Chromosome.show(Population.chromosomes);
//        System.out.println(Population.calibratedFitnessSum);
//        System.out.println(Population.epsilon);
//        System.out.println(Population.min);

    }

}
