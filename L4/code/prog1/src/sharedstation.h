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

    void AttenteEnGare(Locomotive &loco, int nbToursTrain);

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
