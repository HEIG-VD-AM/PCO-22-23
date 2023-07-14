//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Auteurs : Alexis Martins, Anthony David, Pablo Saez

#include "locomotivebehavior.h"
#include "ctrain_handler.h"

bool LocomotiveBehavior::emergencyStopped = false;

void LocomotiveBehavior::run()
{
    //Initialisation de la locomotive
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");

    /* A vous de jouer ! */

    // Vous pouvez appeler les méthodes de la section partagée comme ceci :
    //sharedSection->access(loco);
    //sharedSection->leave(loco);

    int toursActuels = 0;
    bool inverse = false;
    while(true) {
        // On attend qu'une locomotive arrive sur le contact 1.
        // Pertinent de faire ça dans les deux threads? Pas sûr...

        attendre_contact(pointsControleAiguillage[inverse]);
        sectionPartagee->access(loco);
        diriger_aiguillage(aiguillage[inverse].first, aiguillage[inverse].second, 0);

        attendre_contact(pointsControleAiguillagePartages[!inverse]);
        diriger_aiguillage(aiguillage[!inverse].first, aiguillage[!inverse].second, 0);

        attendre_contact(pointsControleAiguillage[!inverse]);
        sectionPartagee->leave(loco);

        attendre_contact(pointDeContact);
        loco.afficherMessage("J'ai atteint le contact");
        toursActuels++;
        gare.AttenteEnGare(loco, toursActuels);

        if(!(toursActuels % toursAvantInverse)) {
            inverse = !inverse;
            loco.inverserSens();
        }

    }
}

void LocomotiveBehavior::printStartMessage()
{
    qDebug() << "[START] Thread de la loco" << loco.numero() << "lancé";
    loco.afficherMessage("Je suis lancée !");
}

void LocomotiveBehavior::printCompletionMessage()
{
    qDebug() << "[STOP] Thread de la loco" << loco.numero() << "a terminé correctement";
    loco.afficherMessage("J'ai terminé");
}
