import java.util.HashMap;
import java.util.Scanner;

/**
 * @author DOCTORY
 */
public class Demo {
    public static final int size = 9;

    public static void main(String[] args) {
        /* 初始化 */
        int[]   parent1 = new int[size];
        int[]   parent2 = new int[size];
        int[]   child1  = new int[size];
        int[]   child2  = new int[size];
        Scanner scanner = new Scanner(System.in);
        for (int i = 0; i < size; i++) {
            parent1[i] = scanner.nextInt();
        }
        for (int i = 0; i < size; i++) {
            parent2[i] = scanner.nextInt();
        }

        HashMap<Integer, Integer> map1 = new HashMap<>(size);
        for (int i = 0; i < size; i++) {
            map1.put(parent1[i], i);
        }
        /* 生成子代 */
        boolean[] visited  = new boolean[size];
        boolean   onesTurn = true;

        for (int i = 0; i < size; i++) {

            if (visited[i]) {
                continue;
            }
            int j = i;
            do {
                visited[j] = true;
                if (onesTurn) {
                    child1[j] = parent1[j];
                    child2[j] = parent2[j];
                } else {
                    child1[j] = parent2[j];
                    child2[j] = parent1[j];
                }
                j = map1.get(parent2[j]);
            } while (j != i);
            onesTurn = !onesTurn;
        }

        /* 输出 */
        for (int i = 0; i < size; i++) {
            System.out.print(child1[i] + " ");
        }
        System.out.println();
        for (int i = 0; i < size; i++) {
            System.out.print(child2[i] + " ");
        }
    }
}
