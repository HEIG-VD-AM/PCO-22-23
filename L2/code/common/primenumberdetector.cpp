/**
*  Fichier  : primenumberdetector.cpp
*  Auteurs  : Alexis Martins et Pablo Saez
*  Date     : 18.10.2022
*
*  Description : Le but général de ce programme est de tester si un nombre est
*  premier ou non. Celui-ci est séparé en plusieurs parties.
*  Dans un premier temps, on retrouve une classe qui n'est pas multi-threadée.
*  Puis dans un second temps, on réalise la même opération avec plusieurs
*  threads.
*
*  Test : On défini un certains nombres de nombres que l'on sait premiers et
*  un certains nombres non-premiers. Si le programme nous offre une réponse
*  cohérente avec le résultat attendu, alors le test est réussi.
*
*  Solution utilisée :
*  - Programme non multi-threadé : Utilisation de l'algorithme fourni en classe.
*  - Programme multi-threadé : Calcul de la racine du nombre à tester. Ensuite
*    on répartit en plages égales les multiples à tester entre les threads.
*    Si un thread trouve un multiple, alors les autres s'arrêtent.
*/

#include "primenumberdetector.h"

///
/// \brief PrimeNumberDetector::isPrime
/// \param number
/// \return True  --> Est un nombre premier
///         False --> N'est pas une nombre premier
///
bool PrimeNumberDetector::isPrime(uint64_t number) {
    if(number <= 1)
           return false;

       uint64_t square = sqrt(number);
       for(size_t i = 2; i < square; ++i) {
           if(!(number % i)) {
               return false;
           }
       }
       return true;
}

/// Constructeur de la classe pour le multri-threading.
/// \brief PrimeNumberDetectorMultiThread::PrimeNumberDetectorMultiThread
/// \param nbThreads Nombre de threads utilisés par le programme.
///
PrimeNumberDetectorMultiThread::PrimeNumberDetectorMultiThread(size_t nbThreads) {

    threadNumber = nbThreads;
}

/// Calcul la taille de la range que doit tester un thread.
/// \brief PrimeNumberDetectorMultiThread::calculateThreadRange
/// \param number Nombre à diviser en un certain nombre de plages.
/// \return Taile de la plage que chaque thread va tester.
///
uint64_t PrimeNumberDetectorMultiThread::calculateThreadRange(uint64_t number) {

    // Divise le nombre selon le nombre de thread donnés au programme
    return (uint64_t) std::ceil(number/threadNumber);
}

/// Initialise les threads et les gère.
/// \brief PrimeNumberDetectorMultiThread::isPrime
/// \param number Nombre potentiellement premier
/// \return True  --> Est un nombre premier
///         False --> N'est pas une nombre premier
///
bool PrimeNumberDetectorMultiThread::isPrime(uint64_t number) {

    if(number <= 1)
           return false;

    numberIsPrime = true;

    // Calcul des différentes varaibles utilisées par le programme
    uint64_t squareRoot = (uint64_t) std::ceil(sqrt(number));
    uint64_t range = calculateThreadRange(squareRoot);

    // Borne inférieure et supérieure utilisée par chaque thread
    uint64_t lowerBound = 2;
    uint64_t upperBound = range > squareRoot ? squareRoot : range;

    for(size_t i = 0; i < threadNumber; ++i) {

        // Initialisation de chaque thread en l'ajoutant dans un vecteur de thread
        threads.push_back(new PcoThread(&PrimeNumberDetectorMultiThread::primeInRange, this, lowerBound, upperBound, number));

        // Calcul des prochaines bornes.
        lowerBound = (i + 1) * range;
        upperBound = (i + 2) * range;
    }

    // Jointure de chaque thread
    for(size_t i = 0; i < threadNumber; ++i) {
        threads.at(i)->join();
    }

    // Remise à zéro du vecteur de threads si cette méthode venait à être ré-utilisée
    threads.clear();

    return numberIsPrime;
}

/// Test de primalité qui vérifie si le nombre que l'on souhaite tester est premier ou non.
/// Méthode utilisée par chaque thread dans la range donnée [lowerBound; upperBound].
/// La fonction s'arrête lorsqu'un des threads a trouvé un multiple du nombre que l'on test.
/// Il le signale aux autres via les boolean  numberIsPrime, vérificaition faite tous les NCheckRange fois.
/// \brief PrimeNumberDetectorMultiThread::primeInRange
/// \param lowerBound Borne inférieure de la range donnée
/// \param upperBound Borne supérieure de la range donnée
/// \param number Nombre potentiellement premier
///
void PrimeNumberDetectorMultiThread::primeInRange(uint64_t lowerBound, uint64_t upperBound, uint64_t number) {


    for(size_t i = lowerBound; i <= upperBound; ++i) {

        if(!numberIsPrime){
            return;
        }
        else if(!(number % i)) {

            numberIsPrime = false;
            return;
        }
    }
}

