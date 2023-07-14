//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Auteurs : Alexis Martins, Anthony David, Pablo Saez

#ifndef SHARED_STATION_H
#define SHARED_STATION_H

#include <pcosynchro/pcosemaphore.h>
#include <pcosynchro/pcothread.h>
#include "ctrain_handler.h"
#include "locomotive.h"

class SharedStation
{


public:
    SharedStation(int nbTours);

    /**
     * @brief AttenteEnGare Indique si lors de l'appel de la fonction s'il y a
     * eu un attente en gare à gérer. Dans ce cas les trains se sont attendus
     * et le mode de priorité inversé.
     * @param loco Locomotive qui passe dans la gare.
     * @param nbToursTrain Nbre de tours actuels de la locomotive.
     * @return Si les trains se sont attendus en gare --> True
     *         S'il ne fallait pas encore s'attendre  --> False
     */
    bool AttenteEnGare(Locomotive &loco, int nbToursTrain);

    /* Implémentez toute la logique que vous avez besoin pour que les locomotives
     * s'attendent correctement à la station */

private:

    /* TODO */

    /**
     * @brief mutex Evite l'accès multiple aux variables partagées
     **/
    PcoSemaphore mutex;

    /**
     * @brief attente Permet d'attendre le second train en gare
     **/
    PcoSemaphore attente;

    /**
     * @brief trainEnAttente Indique si l'autre train attend déjà en gare
     **/
    bool trainEnAttente;

    /**
     * @brief nbTours Nombre de tours avant d'attendre en gare
     **/
    int nbTours;

};

#endif // SHARED_STATION_H
