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
bool LocomotiveBehavior::modePrioInverse = false;
PcoSemaphore LocomotiveBehavior::mutex(1);

void LocomotiveBehavior::run()
{
    srand(time(0));
    //Initialisation de la locomotive
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");

    /* A vous de jouer ! */

    // Vous pouvez appeler les méthodes de la section partagée comme ceci :
    //sharedSection->access(loco);
    //sharedSection->leave(loco);

    int toursActuels = 0;   // Tours totaux réalisés
    bool inverse = false;   // Indique si la loco roule en sens inverse

    /*
     * Explications sur le fonctionnement :
     * - Les locomotives connaissent à chaque fois les différents points pour
     *   activer certaines fonctionnalités (annonce arrivée en section critique,
     *   requête section critique, etc...). Elles connaissent les points dans les
     *   deux sens, sous forme de tableau à  2 éléments. Le premier étant pour la
     *   circulation horaire et le second pour le sens anti-horaire. Le plus simple
     *   est donc d'utiliser un booléen (correspond à inverse) qui indique dans quel
     *   sens va la loco et donc quelle case utilisée pour les points.
     */

    while(true) {



        /*
         * Détecte lorsque la locomotive passe sur le point indiquant
         * qu'elle doit faire la requête pour accéder en section critique.
         */
        attendre_contact(pointsControleAiguillageAccepte[inverse]);
        sectionPartagee->request(loco, priorite);

        /*
         * Indique que la locomotive arrive sur le point d'acceptation pour
         * entrer en section critique. Si celle-ci est acceptée, alors elle
         * change son aiguillage pour qu'il corresponde.
         */
        attendre_contact(pointsControleAiguillageRequete[inverse]);
        sectionPartagee->access(loco, priorite);
        diriger_aiguillage(aiguillage[inverse].first, aiguillage[inverse].second, 0);

        /*
         * Avant la sortie de la section partagée, mise à niveau de l'aiguillage.
         */
        attendre_contact(pointsControleAiguillagePartages[!inverse]);
        diriger_aiguillage(aiguillage[!inverse].first, aiguillage[!inverse].second, 0);

        /*
         * Sortie de la section partagée (après l'aiguillage). Appel de leave pour
         * indiqué la sortie de la section et potentiellement relaché l'autre train.
         */
        attendre_contact(pointsControleAiguillageRequete[!inverse]);
        sectionPartagee->leave(loco);

        /*
         * Le point comptant les tours du train et sa gare sont identiques.
         * On compte alors les tours et on réalise un appel à la gestion de
         * l'attente en gare. Il se peut donc potentiellement que le train reste
         * à attendre le second. Une fois l'attente réalisée, on change la priorité.
         * Il faut cependant vérifier que l'on ne change pas plusieurs fois (2 trains
         * feraient 2 changements), ce qui nous retournerait au point initial.
         */
        attendre_contact(pointDeContact);
        loco.afficherMessage("J'ai atteint le contact");
        toursActuels++;
        if(gare.AttenteEnGare(loco, toursActuels)) {

            // Choix aléatoire de la priorité
            loco.priority = rand() % (MAX_PRIO + 1);

            mutex.acquire();
            // Détermine si la priorité a déjà été inversée par l'autre loco.
            if(!modePrioInverse) {
                sectionPartagee->togglePriorityMode();
                modePrioInverse = true;
            }
            else {
                modePrioInverse = false;
            }
            mutex.release();
        }

        /*
         * Vérifie si le train doit changer de sens suite au tour qu'il vient de
         * réaliser.
         */
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
