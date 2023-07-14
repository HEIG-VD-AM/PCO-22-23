//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Auteurs : Annen Rayane, Martins Alexis

#include "skierbehavior.h"

#include <QRandomGenerator>

int SkierBehavior::nextId = 0;

SkierBehavior::SkierBehavior(PCCarInterface *interface, std::shared_ptr<CableCarSkierInterface> cableCar)
    : _interface(interface),  skierId(nextId++), cableCar(cableCar)
{
    _interface->consoleAppendText(skierId, "Salut, le ski c'est la vie");
}


void SkierBehavior::run()
{
    // A vous d'ajouter le comportement du skieur

    // Chaque skier réalise sa boucle tant que la mise hors-service n'est pas demandée
    while(cableCar->isInService()) {
        cableCar->waitForCableCar(skierId);

        /*
         * Lorsque c'est la fin du service et que des skiers attendent à la station
         * on force l'arrêt de l'attente. La condition ci-dessous permet de
         * récupérer les threads sortant et de forcer l'arrêt de la boucel
         */
        if(!cableCar->isInService()) {
            break;
        }

        cableCar->goIn(skierId);
        cableCar->waitInsideCableCar(skierId);
        cableCar->goOut(skierId);
        cableCar->slideSlope(skierId);
    }

    PcoThread::thisThread()->requestStop();
    return;
}
