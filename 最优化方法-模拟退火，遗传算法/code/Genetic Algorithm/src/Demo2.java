import java.util.Scanner;

public class Demo2 {
    public static void main(String[] args) {
        Scanner scanner=new Scanner(System.in);
        int row=scanner.nextInt();
        int col=scanner.nextInt();
        int[][] values=new int[row][col];
        for (int i=0;i<row;i++) {
            for (int j=0;j<col;j++) {
                values[i][j]=scanner.nextInt();
            }
        }
        int[][] dp     = new int[row][col];

        dp[0][0] = values[0][0];

        for (int i = 1; i < row; i++) {
            dp[i][0] = dp[i-1][0] + values[i][0];
        }
        for (int j = 1; j < col; j++) {
            dp[0][j] = dp[0][j - 1] + values[0][j];
        }
        for (int i = 1; i < row; i++) {
            for (int j = 1; j < col; j++) {
                dp[i][j] = Math.max(dp[i-1][j], dp[i][j - 1]) + values[i][j];
            }
        }
        System.out.println(dp[row-1][col-1]);
    }


}
