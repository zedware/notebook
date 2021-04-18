/*

https://programmingpraxis.com/2009/10/23/mr-s-and-mr-p/

John McCarthy, who discovered Lisp, attributes this puzzle to Hans Freudenthal:

We pick two numbers a and b, so that 99 ≥ a ≥ b ≥ 2. We tell Mr. P. the product
a × b and Mr. S. the sum a + b. Then Mr. S. and Mr. P. engage in the following
dialog:

Mr. P.: I don’t know the numbers.
Mr. S.: I knew you didn’t know. I don’t know either.
Mr. P.: Now I know the numbers.
Mr. S.: Now I know them too.

Find the numbers a and b.

Your task is to find the two numbers. When you are finished, you are welcome to
read or run a suggested solution, or to post your solution or discuss the
exercise in the comments below.
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 打印更多的信息
bool g_debug = false;

// 计算数组中的元素个数
#define NARRAY(x) (sizeof(x) / sizeof(x[0]))
#define NMIN 2   // Can be changed to 2 or 3, or even bigger
#define NMAX 99  // If changed, adjust the g_primes[] please.

// 实现计算好的[NMIN, NMAX]之间的质数表
const int g_primes[] = { 2,  3,  5,  7,  11, 13, 17, 19, 23, 29, 31, 37, 41,
                         43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97 };

// 用两个位图来表示S和P集合。为true表示对应的数在集合内。
// 程序的效率不太高，因为S中有200个数；P中有1万个数。
bool g_s_bitmap[NMAX * 2 + 1];
bool g_p_bitmap[NMAX * NMAX + 1];

// 检查v是否为质数表中的质数
bool IsPrime(int v)
{
    for (size_t i = 0; i < NARRAY(g_primes); i++) {
        if (v == g_primes[i])
            return true;
    }

    return false;
}

// 检查v是否在S集合中
bool InSum(int v)
{
    // 优化：S集合中没有偶数
    if (v % 2 == 0)
        return false;

    for (int i = NMIN; i < NMAX * 2 + 1; i++) {
        if (g_s_bitmap[i] && v == i)
            return true;
    }

    return false;
}

// 第一步：检查P集合中的数
// 因数分解只有一种结果的：要么是两个质数相乘，要么是特殊情况。
// 例如从3开始的时候，20只能分解为4*5，而不能分解为2*10。
bool CheckProdStep1(int v)
{
    // P集合中的数分解为合法的M*N的个数
    int count = 0;
    for (int i = NMIN; i <= v / 2; i++) {
        for (int j = i; j <= v / 2; j++) {
            if (i * j == v) {
                count++;
                if (IsPrime(i) && IsPrime(j))
                    return false;
            }
        }
    }

    return count > 1;
}

// 第二步：检查S中的数是否合法
// 1）题目范围内的偶数必然可以分解为两个质数之和；不过奇数也有可能。
// 2) 最小的几个数或最大的几个数都可能只有一种M+N组合。
bool CheckSumStep2(int v)
{
    // 优化：S集合中没有偶数
    if (v % 2 == 0)
        return false;

    // 记录S中的数的合法M+N组合有几个
    int count = 0;
    for (int i = NMIN; i < NMAX + 1; i++) {
        for (int j = i; j < NMAX + 1; j++) {
            if (i + j != v)
                continue;

            count++;

            // 检查S的M+N组合的M*N是否在P集合内
            if (!g_p_bitmap[i * j])
                return false;
        }
    }

    return (count > 1);
}

// 第三步：检查P集合中的数是否合法
bool CheckProdStep3(int v)
{
    int count = 0;
    int x, y;

    for (int i = NMIN; i <= v / 2; i++) {
        for (int j = i; j <= v / 2; j++) {
            if (i * j != v)
                continue;

            int sum = i + j;
            if (InSum(sum)) {
                // 已经有一个M*N的组合在S集合了
                if (count == 1)
                    return false;

                count++;
                x = i;
                y = j;
            }
        }
    }

    if (count == 1 && g_debug) {
        printf("%d + %d = %d, ", x, y, x + y);
        printf("%d x %d = %d\n", x, y, v);
    }

    return (count == 1);
}

// 第四步：检查集合S中的数是否合法
bool CheckSumStep4(int v)
{
    int count = 0;
    int x, y;
    for (int i = NMIN; i < v; i++) {
        for (int j = i; j < v; j++) {
            if (i + j != v)
                continue;

            if (g_p_bitmap[i * j]) {
                if (count == 1)
                    return false;

                count++;
                x = i;
                y = j;
            }
        }
    }

    if (count == 1 && g_debug) {
        printf("%d + %d = %d, ", x, y, x + y);
        printf("%d * %d = %d\n", x, y, x * y);
    }

    return (count == 1);
}

void PrintP()
{
    printf("P:");
    for (int i = NMIN * NMIN; i < NMAX * NMAX + 1; i++) {
        if (!g_p_bitmap[i])
            continue;
        printf(" %d", i);
    }
    printf("\n");
}

void PrintS()
{
    printf("S:");
    for (int i = NMIN; i < NMAX + 1; i++) {
        if (!g_s_bitmap[i])
            continue;
        printf(" %d", i);
    }
    printf("\n");
}

//
// 把P和S的每一句对话都翻译成下面的解释。
// 1. 集合P，去掉因数分解后只有一种x*y结果的（因为P先生可以立即得出结论）。
//    优化：可以先计算出所有质数，将质数以及质数*质数的位置设为false。
// 2. 集合S，去掉只有一种M+N的（因为S先生可以立即得出结论）。
//    M*N必须要在P里（不然P先生早就做出了结论，也意味着去掉可分解为两个质数之和的）。
// 3. 集合P，去掉因数分解后的组合x,y的和落在S中有多个的。
//    (P先生可以得出结论说明落在S中的只有一个。)
// 4. 集合S，去掉x+y的组合在P中有多个的。
//    (S先生可以得出结论说明落在P中的只有一个。)
int main(void)
{
    memset(g_p_bitmap, 0, NMAX * NMAX + 1);
    // 1. 集合P，去掉因数分解后只有一种x*y结果的（因为P先生可以立即得出结论）。
    //    优化：可以先计算出所有质数，将质数以及质数*质数的位置设为false。
    for (int i = NMIN * NMIN; i < NMAX * NMAX + 1; i++) {
        g_p_bitmap[i] = CheckProdStep1(i);
    }

    PrintP();

    memset(g_s_bitmap, 0, NMAX + 1);
    // 2. 集合S，去掉只有一种M+N的（因为S先生可以立即得出结论）。
    //    M*N必须要在P里（不然P先生早就做出了结论，也意味着去掉可分解为两个质数之和的）。
    for (int i = NMIN; i < NMAX * 2 + 1; i++) {
        g_s_bitmap[i] = CheckSumStep2(i);
    }

    // S: 11 17 23 27 29 35 37 41 47 51 53 57 59 65 67 71 77 79 83 87 89 93 95
    // 97 有一种说法似乎允许M和N可以出现多次，导致 29、35
    // 等不合格。这里未考虑。例如：
    //  29 = 7 + 11 + 11, 35 = 7 + 11 + 17,
    PrintS();

    // 3. 集合P，去掉因数分解后的组合x,y的和落在S中有多个的。
    for (int i = NMIN * NMIN; i < NMAX * NMAX + 1; i++) {
        g_p_bitmap[i] = CheckProdStep3(i);
    }

    PrintP();

    // 4. 集合S，去掉x+y的组合在P中有多个的。
    for (int i = NMIN; i <= NMAX; i++) {
        if (g_s_bitmap[i]) {
            g_s_bitmap[i] = CheckSumStep4(i);
            // if (g_s_bitmap[i]) printf("FOUND: %d\n", i);
        }
    }

    PrintS();
    return 0;
}
