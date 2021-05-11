#include "primes.h"

// Hardcoded bases for Miller-Rabin tests.
const std::vector<std::vector<uint_fast64_t>> MR_BASES = {
    {31, 73},
    {2, 299417},
    {2, 9332593},
    {11000544, 31481107},
    {2, 7, 61},
    {2, 1005905886, 1340600841},
    {2, 642735, 553174392, 3046413974},
    {2, 75088, 642735, 203659041, 3613982119},
    {2, 325, 9375, 28178, 450775, 9780504, 1795265022}};

// mulmod and powmod taken from https://codereview.stackexchange.com/a/155483
// Computes a * b (mod m) without overflow as long as a and b aren't both very close to 2^64
uint_fast64_t mulmod(uint_fast64_t a, uint_fast64_t b, uint_fast64_t m) {
    if (a <= UINT_FAST32_MAX && b <= UINT_FAST32_MAX) return (a * b) % m;

    uint_fast64_t r = 0;

    while (a > 0) {
        if (a & 1)
            if ((r += b) > m) r %= m;
        a >>= 1;
        if ((b <<= 1) > m) b %= m;
    }

    return r;
}

// mulmod and powmod taken from https://codereview.stackexchange.com/a/155483
// Computes a ^ b (mod m) without overflow as long as mulmod doesn't overflow
uint_fast64_t powmod(uint_fast64_t a, uint_fast64_t n, uint_fast64_t m) {
    uint_fast64_t r = 1;

    while (n > 0) {
        if (n & 1)
            r = mulmod(r, a, m);
        a = mulmod(a, a, m);
        n >>= 1;
    }
    return r;
}

// If N is on the order of 2^14 (specifically N <= 23001), essentially do a lookup table.
// For n > 23001, returns true if n is not divisible by any prime below 50.
bool isSmallPrime(uint_fast64_t n)
{
    if (n == 2 || n == 3 || n == 5)
        return true;
    if (n < 2 || (n % 2) == 0 || (n % 3) == 0 || (n % 5) == 0)
        return false;
    if (n < 49)
        return true;
    if ((n % 7) == 0 || (n % 11) == 0 || (n % 13) == 0 || (n % 17) == 0 ||
        (n % 19) == 0 || (n % 23) == 0 || (n % 29) == 0 || (n % 31) == 0 ||
        (n % 37) == 0 || (n % 41) == 0 || (n % 43) == 0 || (n % 47) == 0)
        return false;
    if (n < 2809)
        return true;
    if (n <= 23001)
        return powmod(2, n, n) == 2 && n != 7957 && n != 8321 && n != 13747 && n != 18721 && n != 19951;
    return true;
}

// A single Miller-Rabin test for a given base, s, and t.
bool mrTest(uint_fast64_t n, uint_fast64_t base, int s, uint_fast64_t t)
{
    uint_fast64_t b = powmod(base, t, n);
    if (b == 1 || b == n - 1)
        return true;
    for (int j = 1; j < s; j++)
    {
        b = powmod(b, 2, n);
        if (b == n - 1)
            return true;
        if (b == 1)
            return false;
    }
    return false;
}

// Miller rabin test on n with a vector of bases from MR_BASES.
// Skips unnecessary checks by assuming that n went through isSmallPrime checks.
bool millerRabin(uint_fast64_t n, int basesIdx)
{
    uint_fast64_t t = n - 1;
    int s = 0;
    while ((t & 1) == 0)
    {
        t >>= 1;
        s++;
    }

    for (uint_fast64_t base : MR_BASES[basesIdx])
    {
        if (base >= n)
            base %= n;
        if (base >= 2)
        {
            if (!mrTest(n, base, s, t))
                return false;
        }
    }

    return true;
}

// For a nonnegative integer n, determine if n is prime.
// Note that this will run into overflow errors for values close to 2^64.
bool isPrime(uint_fast64_t n)
{
    bool initialCheck = isSmallPrime(n);
    if (n <= 23001 || !initialCheck)
        return initialCheck;
    if (n < 9080191)
        return millerRabin(n, 0);
    if (n < 19471033)
        return millerRabin(n, 1);
    if (n < 38010307)
        return millerRabin(n, 2);
    if (n < 316349281)
        return millerRabin(n, 3);
    if (n < 4759123141)
        return millerRabin(n, 4);
    if (n < 105936894253)
        return millerRabin(n, 5);
    if (n < 31858317218647)
        return millerRabin(n, 6);
    if (n < 3071837692357849)
        return millerRabin(n, 7);
    return millerRabin(n, 8);
}

// Overloads
bool isPrime(uint_fast32_t n)
{
    return isPrime((uint_fast64_t)n);
}

bool isPrime(uint_fast16_t n)
{
    return isPrime((uint_fast64_t)n);
}

bool isPrime(int_fast64_t n)
{
    if (n <= 3)
        return n > 1;
    return isPrime((uint_fast64_t)n);
}

bool isPrime(int_fast32_t n)
{
    if (n <= 3)
        return n > 1;
    return isPrime((uint_fast64_t)n);
}

bool isPrime(int_fast16_t n)
{
    if (n <= 3)
        return n > 1;
    return isPrime((uint_fast64_t)n);
}
