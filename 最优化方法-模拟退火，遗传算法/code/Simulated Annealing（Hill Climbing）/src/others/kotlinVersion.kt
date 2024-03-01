import java.io.File
import java.io.FileNotFoundException
import java.io.PrintWriter
import java.util.*
import java.util.function.Consumer
import kotlin.math.exp

/*** @author 董若扬  */
object KotlinVersion {
    /*** 获取CPU可用线程数 */
    private val AVAILABLE_PROCESSORS = Runtime.getRuntime().availableProcessors()
    private const val REPETITION_TIMES = 8

    /*** 模拟退火参数设置: 初始温度, 临界温度, 降温系数, 同一温度下重复次数, 同一用例处理次数. */
    const val INITIAL_TEMPERATURE = 1500.0
    const val CRITICAL_TEMPERATURE = 1e-4
    const val SA_RATIO = 0.99
    const val REPETITIONS_SAME_TEMPERATURE = 206
    private val REPETITIONS_SAME_SAMPLE = REPETITION_TIMES * AVAILABLE_PROCESSORS

    /*** 登山算法参数设置: 尝试攀登次数. */
    const val CLIMBING_TIMES = 300000

    /*** 工件(行)数, 机器(列)数. */
    var Row_Workpiece = 0
    var Column_Machine = 0

    /*** 用例所在文件的路径, 保存答案的文件的路径 */
    private val FILE_INPUT = File("C:\\Users\\DOCTORY\\Desktop\\SA10.txt")
    private val FILE_OUTPUT = File("C:\\Users\\DOCTORY\\Desktop\\hill\\ANS10____.txt")
    var rand = Random()
    private var printWriter: PrintWriter? = null

    /* 实例化PrintWrite类 */
    init {
        try {
            printWriter = PrintWriter(FILE_OUTPUT)
        } catch (e: FileNotFoundException) {
            e.printStackTrace()
        }
    }

    /*** 备份用例中各工件在各机器上需要的时间 */
    lateinit var data: Array<IntArray>

    /*** THREADS列表保存正在运行的线程, 用于线程管理; RESULT列表保存每次计算得到的结果 */
    val THREADS: MutableList<Thread> = ArrayList()
    val RESULTS: MutableList<Int> = ArrayList()

    @JvmStatic
    fun main(args: Array<String>) {
        /* 标记开始时间 */
        val start = System.currentTimeMillis()
        try {
            Scanner(FILE_INPUT).use { scanner ->
                /* 数据初始化: 输入工件数,机器数,时间表. */Row_Workpiece = scanner.nextInt()
                Column_Machine = scanner.nextInt()
                data = Array(Row_Workpiece) { IntArray(Column_Machine) }
                for (i in 0 until Row_Workpiece) {
                    for (j in 0 until Column_Machine) {
                        scanner.nextInt()
                        data[i][j] = scanner.nextInt()
                    }
                }
                /* 检查用例是否读取完整 */if (scanner.hasNext()) {
                println("ERROR" + scanner.nextInt())
            } else {
                println("SUCCEED")
            }
            }
        } catch (e: FileNotFoundException) {
            e.printStackTrace()
        }
        /* 开始多线程计算,以AMD Ryzen 7 4800H为例, 每次运行16个线程, 重复REPETITION_TIMES次,共计得到REPETITIONS_SAME_SAMPLE个结果 */for (k in 0 until REPETITIONS_SAME_SAMPLE) {
            /* 创建一个线程并启动 */
            val thread = Thread(IndividualTask())
            thread.start()
            /* 将线程加入到用于现场管理的列表中 */THREADS.add(thread)
            /* 如果正在同时运行的线程数达到了CPU可用的最大线程数,等待当前正在处理的多个线程完成任务再添加新线程 */if (THREADS.size % AVAILABLE_PROCESSORS == 0) {
                waitForThreads()
            }
        }
        /* 标记结束时间 */
        val end = System.currentTimeMillis()
        printResult(start, end)
    }

    private fun waitForThreads() {
        for (thread in THREADS) {
            try {
                thread.join()
            } catch (e: InterruptedException) {
                e.printStackTrace()
            }
        }
        THREADS.clear()
    }

    private fun printResult(start: Long, end: Long) {
        /* 计算程序运行时间 */
        val totalMillisecond = end - start
        val currentMillisecond = totalMillisecond % 1000
        val totalSecond = totalMillisecond / 1000
        val currentSecond = totalSecond % 60
        val totalMinutes = totalSecond / 60
        val currentMinute = totalMinutes % 60
        /* 输出程序总运行时间,格式:<分>:<秒>:<毫秒>.并将时间输出到控制台和输出文件中 */println("Time:$currentMinute:$currentSecond:$currentMillisecond")
        printWriter!!.println("Time:$currentMinute:$currentSecond:$currentMillisecond")
        /* 将输出结果按自然顺序排序(即升序),并输出到文件 */RESULTS.sortWith(Comparator.naturalOrder())
        RESULTS.forEach(Consumer { x: Int? ->
            printWriter!!.println(
                x!!
            )
        })
        printWriter!!.close()
    }

    /*** 实现Runnable接口, 以实现多线程 */
    internal class IndividualTask : Runnable {
        /* 当前工件顺序加工所需时间,随机交换两个工件顺序后加工所需时间 */
        private var currentTime = 0
        private var nextTime = 0

        /* 随机交换的两个工件的索引 */
        private var exchangeIndex1 = 0
        private var exchangeIndex2 = 0

        /* 初始化温度 */
        private var t = INITIAL_TEMPERATURE

        /* 实例化当前加工时间表和交换后的加工时间表 */
        private val current = Array(Row_Workpiece) { IntArray(Column_Machine) }
        private val next = Array(Row_Workpiece) { IntArray(Column_Machine) }

        /* 实例化一个线程时初始化时间表 */
        init {
            for (i in 0 until Row_Workpiece) {
                for (j in 0 until Column_Machine) {
                    next[i][j] = data[i][j]
                    current[i][j] = next[i][j]
                }
            }
        }

        /* 重写Runnable接口中的run方法,里面实现了多线程运行的代码 */  override fun run() {
            simulatedAnnealing(current, next)
            //            hillClimbing(current, next);
            /*将结果保存到RESULT列表中,并在控制台输出当前线程的运行结果*/RESULTS.add(currentTime)
            println(Thread.currentThread().name + " " + currentTime)
        }

        /* 登山算法 */
        private fun hillClimbing(current: Array<IntArray>, next: Array<IntArray>) {
            currentTime = getMinTimeCost(current)
            /* 开始登山 */for (i in 0 until CLIMBING_TIMES) {
                /* 随机交换两个工件的加工顺序,并求最小工作时间 */
                randomExchange(next)
                nextTime = getMinTimeCost(next)
                /* 如果交换后工作时间更短,保存这种更改,否则还原这种更改. */if (nextTime - currentTime < 0) {
                    currentTime = nextTime
                    exchange(current)
                } else {
                    exchange(next)
                }
            }
        }

        /*** 模拟退火算法 */
        private fun simulatedAnnealing(current: Array<IntArray>, next: Array<IntArray>) {
            /* 改变后的工作时间与当前工作时间的差值,如果工件的加工顺序改变后工作时间变短,该值为负数 */
            var delta: Int
            currentTime = getMinTimeCost(current)
            /* 当温度高于临界温度时运行模拟退火算法 */while (t > CRITICAL_TEMPERATURE) {
                /* 每个温度下重复操作多次 */
                for (num in 0 until REPETITIONS_SAME_TEMPERATURE) {
                    /* 随机交换两个工件的加工顺序,并求最小工作时间 */
                    randomExchange(next)
                    nextTime = getMinTimeCost(next)
                    delta = nextTime - currentTime
                    if (delta < 0) {
                        /* 如果交换工件加工顺序后加工时间更短了,保存这一有利改变 */
                        currentTime = nextTime
                        exchange(current)
                    } else {
                        /* 否则生成一个[0,1)的随机数,以决定是否尝试这一"更坏"的加工顺序 */
                        val r = rand.nextDouble()
                        if (r < exp(-delta / t)) {
                            /* 有 p=exp(-delta / t)的概率接受这一"更坏"的加工顺序 */
                            currentTime = nextTime
                            exchange(current)
                        } else {
                            /* 否则恢复原先加工顺序 */
                            exchange(next)
                        }
                    }
                }
                t *= SA_RATIO
            }
        }

        /*** 随机交换两个工件的加工顺序 */
        private fun randomExchange(next: Array<IntArray>) {
            do {
                exchangeIndex1 = rand.nextInt(Row_Workpiece)
                exchangeIndex2 = rand.nextInt(Row_Workpiece)
            } while (exchangeIndex1 == exchangeIndex2)
            exchange(next)
        }

        /*** 交换两个工件的加工顺序,既用于next时间表的随机尝试,也用于current时间表保存next时间表的尝试,还用于next时间表恢复原状. */
        private fun exchange(next: Array<IntArray>) {
            var temp: Int
            for (i in 0 until Column_Machine) {
                temp = next[exchangeIndex1][i]
                next[exchangeIndex1][i] = next[exchangeIndex2][i]
                next[exchangeIndex2][i] = temp
            }
        }

        companion object {
            /*** 当前加工顺序下的最小工作时间为从时间表的左上角按"向下"和"向右"的方法走到时间表的右下角的最大值,相关说明见报告 */
            private fun getMinTimeCost(value: Array<IntArray>): Int {
                /* 典型的动态规划问题,采用动态规划求解 */
                val dp = Array(Row_Workpiece) { IntArray(Column_Machine) }
                dp[0][0] = value[0][0]
                /* 因为第一行的格子只能从第一行中此前的格子以"向右"的方法到达,第一列同理.避免循环中额外的判断,将第一行和第一列单独计算. */for (i in 1 until Row_Workpiece) {
                    dp[i][0] = dp[i - 1][0] + value[i][0]
                }
                for (i in 1 until Column_Machine) {
                    dp[0][i] = dp[0][i - 1] + value[0][i]
                }
                for (i in 1 until Row_Workpiece) {
                    for (j in 1 until Column_Machine) {
                        dp[i][j] = dp[i - 1][j].coerceAtLeast(dp[i][j - 1]) + value[i][j]
                    }
                }
                /* 时间表右下角的值即为当前加工顺序下的最小工作时间 */return dp[Row_Workpiece - 1][Column_Machine - 1]
            }
        }
    }
}