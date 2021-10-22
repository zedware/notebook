// How to compile and run:
// $ gcc -Wall -g cointoss.c -o cointoss
// $ ./cointoss
//
// How to avoid overflow:
// 1. 64 bit integers, int64_t, uint64_t
// 2. 128 bit integers, __int128_t, __uint128_t.
// 3. Use purpose-built libraries for unlimited numbers.
//
// How to find the bottleneck of performance:
// 1. Analyze the algorithm manually, for example,
// 1) power => power2, for 2^x, bit shift is much faster.
// 2) basic_factorial => factorial, recursive with table lookup
//    Can also pre-compute all of the n! for n in [1, x], and save
//    results in a table. Left as exercise.
//
// 2. Use perf tools (Linux only).
// $ sudo perf record ./cointoss  # Run to the end or Ctrl^C to interrupt
// $ sudo perf report
/*
$ sudo perf record ./cointoss
[sudo] password for wsl:
   2: 1               1
   4: 2               2
   6: 5               5
   8: 14              14
  10: 42              42
  12: 132             132
  14: 429             429
  16: 1430            1430
  18: 4862            4862
  20: 16796           16796
  22: 58786           58786
  24: 208012          208012
  26: 742900          742900
  28: 2674440         2674440
^C[ perf record: Woken up 59 times to write data ]
[ perf record: Captured and wrote 14.504 MB perf.data (380158 samples) ]

$ sudo perf report
Samples: 380K of event 'cpu-clock', Event count (approx.): 95039500000
Overhead  Command   Shared Object      Symbol
  48.67%  cointoss  cointoss           [.] even
  33.19%  cointoss  cointoss           [.] int2bitstring
  13.83%  cointoss  cointoss           [.] noexceed
   2.52%  cointoss  cointoss           [.] head_noexceed_tail
   1.77%  cointoss  cointoss           [.] power2
   0.00%  cointoss  [kernel.kallsyms]  [k] __softirqentry_text_start
   0.00%  cointoss  [kernel.kallsyms]  [k] update_sd_lb_stats.constprop.0
   0.00%  cointoss  [kernel.kallsyms]  [k] __lock_text_start
   0.00%  cointoss  [kernel.kallsyms]  [k] idle_cpu
   0.00%  cointoss  [kernel.kallsyms]  [k] clear_page_erms
   0.00%  cointoss  [kernel.kallsyms]  [k] queue_work_on
   0.00%  cointoss  [kernel.kallsyms]  [k] rcu_core
   0.00%  cointoss  [kernel.kallsyms]  [k] vma_interval_tree_insert
   0.00%  cointoss  ld-2.33.so         [.] 0x0000000000019fdc

The top three bottlenecks:
  48.67%  cointoss  cointoss           [.] even
  33.19%  cointoss  cointoss           [.] int2bitstring
  13.83%  cointoss  cointoss           [.] noexceed
*/

#include <assert.h>
#include <stdio.h>

#define MAX_NUMBER 100

// x^y
// 1. size_t make sure both x and y >= 0.
// 2. __uint128_t is 128 bit unsigned integer.
__uint128_t power(size_t x, size_t y)
{
    __uint128_t base = 1;

    for (; y > 0; y--) {
        // *= is better than `base = base * x'.
        base *= x;
    }

    return base;
}

// 2^y
// Use bit shift
__uint128_t power2(size_t y)
{
    assert(y <= 127);
    return (__uint128_t)1 << y;
}

void test_power2(void)
{
    printf("%s\n", __func__);
    for (size_t n = 1; n < MAX_NUMBER; n++) {
        assert(power(2, n) == power2(n));
    }
}

// Convert a n-bit number to binary string
void int2bitstring(int number, int coin[], int n)
{
    for (int i = 1; number > 0; i++) {
        coin[n - i] = (number % 2 == 0) ? 0 : 1;
        number /= 2;
    }
}

int even(int coin[], int n)
{
    int head = 0, tail = 0;
    for (int i = 0; i < n; i++) {
        if (coin[i] == 1) {
            head++;
        } else {
            tail++;
        }
    }

    if (head == tail) {
        return 1;
    }

    return 0;
}

int noexceed(int coin[], int n)
{
    for (int i = 0; i < n; i++) {
        int head = 0, tail = 0;

        for (int j = 0; j <= i; j++) {
            if (coin[j] == 1) {
                head++;
            } else {
                tail++;
            }
        }

        if (head > tail) {
            return 0;
        }
    }

    return 1;
}

int head_noexceed_tail(int n)
{
    // odd number?
    if (n % 2 == 1) {
        return -1;
    }

    int coin[n];
    for (int i = 0; i < n; i++) {
        coin[i] = 0;
    }

    __uint128_t test = 0;
    int count = 0;
    for (test = 0; test < power2(n); test++) {
        int2bitstring(test, coin, n);
        if (even(coin, n) && noexceed(coin, n)) {
            count++;
        }
    }

    return count;
}

// It is slow to calculate from 1 to n.
__uint128_t basic_factorial(long long n)
{
    __uint128_t fact = 1;

    for (; n > 0; n--) {
        fact = fact * n;
    }

    return fact;
}

// Speedup it with a table of results.
__uint128_t factorial(long long n)
{
    // static variables defaults to zero.
    static __uint128_t results[MAX_NUMBER];

    assert(n > 0 && n < MAX_NUMBER);
    if (n == 1) {
        results[n] = 1;
    } else {
        if (results[n] == 0) {
            results[n] = n * factorial(n - 1);
        }
    }
    return results[n];
}

void test_factorial(void)
{
    printf("%s\n", __func__);
    for (long long n = 1; n < MAX_NUMBER; n++) {
        printf("%Lf, %Lf\n", (long double)basic_factorial(n), (long double)factorial(n));
    }
}

__uint128_t product(long long p, long long q)
{
    __uint128_t prod = 1;

    for (long long i = p + 1; i <= q; ++i) {
        prod = prod * i;
    }

    return prod;
}

long long int expected_head_noexceed_tail(long long n)
{
    if (n % 2 == 1) {
        return -1;
    }

    __uint128_t result = product(n / 2, n) / factorial(n / 2) / (n / 2 + 1);
    return result;
}

int main(void)
{
    // test_power2();
    // test_factorial();

    for (int i = 1; i < MAX_NUMBER; i++) {
        int v = 2 * i;
        printf("% 4d: ", v);
        printf("%d", head_noexceed_tail(v));
        printf("%16lld\n", expected_head_noexceed_tail(v));
    }
    return 0;
}
