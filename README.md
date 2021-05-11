# SmallPrimesCPP
> A small, lightweight, fast, no-nonsense prime checker in C++.

This "library" is an implementation of Miller-Rabin prime testing for unsigned 64-bit integers.
The implementation is largely inspired by SymPy's prime checker. I wrote this and put it up because
I wanted something faster than the naive method (found in small_primes.cpp) but that I could just
throw in a project and use. This is about 10x as fast as the naive method for counting primes below 2^26.

# Usage

Copy the primes.cpp and primes.h files into your directory of choice. Then the prime tester can be
used with the `isPrime` method.

```cpp
#include <iostream>
#include "primes.h"

int main() {
    std::cout << isPrime(67108859) << std::endl; // True
}
```

Note that due to overflow, results can be inaccurate for n near 2^64.

Alternatively, if you just want to grab a method & go, check out small_primes.cpp for a quick
implementation of the naive method.
