//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Auteurs : Annen Rayane, Martins Alexis

#include "cablecarbehavior.h"
#include <iostream>
void CableCarBehavior::run()
{
    // A vous d'ajouter le comportement du télécabine

    // La cabine réalise sa boucle tant que la mise hors-service n'est pas demandée
    while(cableCar->isInService()) {

        cableCar->loadSkiers();
        cableCar->goUp();
        cableCar->unloadSkiers();
        cableCar->goDown();
    }

    PcoThread::thisThread()->requestStop();
    return;
}
