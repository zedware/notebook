#include <stdio.h>

int power(int x, int y) {
    int base = 1;

    for (; y > 0; y--) {
        base = base * x;
    }

    return base;
}

void inttostring(int number, int coin[], int n) {
    for (int i = 1; number > 0; i++) {
        coin[n - i] = (number % 2 == 0) ? 0 : 1;
        number = number / 2;
    }
}

int even(int coin[], int n) {
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

int Noexceed(int coin[], int n) {
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

int headNoexceedtail(int n) {
    if (n % 2 == 1) {
        return -1;
    }

    int coin[n];

    for (int i = 0; i < n; i++) {
        coin[i] = 0;
    }

    int test = 0, count = 0;

    for (test = 0; test < power(2, n); test++) {
        inttostring(test, coin, n);
        if (even(coin, n) && Noexceed(coin, n)) {
            count++;
        }
    }

    return count;
}

long long int factorial(long long n) {
    int fact = 1;

    for (; n > 0; n--) {
        fact = fact * n;
    }

    return fact;
}

long long product(long long p, long long q) {
    long long prod = 1;

    for (long long i = p + 1; i <= q; ++i) {
        prod = prod * i;
    }

    return prod;
}

long long int expectedheadNoexceedtail(long long n) {
    if (n % 2 == 1) {
        return -1;
    }

    return product(n / 2, n) / factorial(n / 2) / (n / 2 + 1);
}

int main(void) {
    for (int i = 1; i <= 20; i++) {
        printf("%d", headNoexceedtail(2 * i));
        printf("%8lld\n", expectedheadNoexceedtail(2 * i));
    }

    return 0;
}
