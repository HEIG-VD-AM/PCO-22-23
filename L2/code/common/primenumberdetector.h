#ifndef PRIMENUMBERDETECTOR_H
#define PRIMENUMBERDETECTOR_H

#include "logging.h"

#include <cstdint>
#include <cstddef>
#include <cmath>
#include <pcosynchro/pcothread.h>

class PrimeNumberDetectorInterface
{
public:
    virtual bool isPrime(uint64_t number) = 0;
};

class PrimeNumberDetector : public PrimeNumberDetectorInterface
{
public:
    bool isPrime(uint64_t /*number*/) override;
};

class PrimeNumberDetectorMultiThread : public PrimeNumberDetectorInterface
{
    // Tableau contenant tous les threads utilisés par le programme
    std::vector<PcoThread*> threads;

    /* Bool indiquant si un nombre est premier ou non. On part du postulat que
    tous les nombres sont premiers */
    bool numberIsPrime = true;

    // Nombre de threads utilisés dans le programme
    size_t threadNumber;

public:
    PrimeNumberDetectorMultiThread(size_t /*nbThreads*/);

    uint64_t calculateThreadRange(uint64_t number);

    bool isPrime(uint64_t /*number*/) override;

    void primeInRange(uint64_t lowerBound, uint64_t upperBound, uint64_t number);
};

#endif // PRIMENUMBERDETECTOR_H
