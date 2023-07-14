/**
*  Fichier : mythread.cpp
*  Auteur  : Alexis Martins
*  Date    : 05.10.2022
*
*  Description : Permet de mettre en évidence le problème d'accès concurent
*  sur une variable partagée.
*
*  Test : Rentrer des valeurs arbitraires et avoir un ratio de 100%. Pour avoir
*  de plus grandes chances d'avoir des valeurs aberrantes, il faut utiliser
*  de grandes valeurs en entrer.
*
*  Solution utilisée : En fouillant la libraire atomic qui nous a été indiquée,
*  j'ai trouvé que chaque type de variable avait une version "atomique". Ceci
*  permet de gérer la concurrence sur une même variable. Si un thread écrit,
*  alors les autres threads essayent de lire, le comportement est défini et
*  les threads attendent les uns sur les autres.
*
*  Autres solutions testées :
*  - Ajout d'un locker avec un boolean static, les résultats approximent les
*    90% de réussites en moyenne. On arrive même à augmenter de quelques
*    pourcents en ajoutant volatile.
*  - Ajout d'une barrière mémoire, la mise en place était assez complexe à
*    comprendre et les résultats obtenus n'étaient pas assez suffisants.
*/

#include "mythread.h"
#include <atomic>

// Modfication de la variable pour la rendre atomique
static volatile std::atomic<long unsigned int> counter;

void runTask(unsigned long nbIterations)
{
    std::atomic<long unsigned int> i = 0;

    while (i < nbIterations)
    { 
        counter++;
        i++;
    }
}

void initCounter()
{
    counter = 0;
}

long unsigned int getCounter()
{
    return counter;
}
