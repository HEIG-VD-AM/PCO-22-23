//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Auteurs : Alexis Martins, Anthony David, Pablo Saez

#include <chrono>
#include <thread>
#include "locomotivebehavior.h"
#include "sharedstation.h"


SharedStation::SharedStation(int nbTours) : mutex(1), attente(0), trainEnAttente(false), nbTours(nbTours)  {
}

bool SharedStation::AttenteEnGare(Locomotive &loco , int nbToursTrain) {

    mutex.acquire();

    // Vérifie que le train ait fait tous ses tours
    if(!(nbToursTrain % nbTours)) {

        afficher_message(qPrintable(QString("The engine no. %1 is waiting at the station.").arg(loco.numero())));

        // Si l'autre train attend déjà, alors on se prépare à repartir.
        if(trainEnAttente) {
            // Attente de 2 sec. suivie du départ des deux trains.
            loco.arreter();
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            attente.release();
            trainEnAttente = false;
        } else {
            // Bloque la circulation du train qui est le premier à arriver.
            trainEnAttente = true;
            loco.arreter();
            mutex.release();
            attente.acquire();
            mutex.acquire();
        }

        if(!LocomotiveBehavior::emergencyStopped) {
            loco.demarrer();
        }

        mutex.release();
        return true;
    }

    mutex.release();
    return false;
}
