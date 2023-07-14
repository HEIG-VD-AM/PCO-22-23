//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Auteurs : Alexis Martins, Anthony David, Pablo Saez


#ifndef SHAREDSECTION_H
#define SHAREDSECTION_H

#include <QDebug>

#include <pcosynchro/pcosemaphore.h>

#include "locomotive.h"
#include "locomotivebehavior.h"
#include "ctrain_handler.h"
#include "sharedsectioninterface.h"
#include "pcosynchro/pcosemaphore.h"
/**
 * @brief La classe SharedSection implémente l'interface SharedSectionInterface qui
 * propose les méthodes liées à la section partagée.
 */
class SharedSection final : public SharedSectionInterface
{

private:
    /* A vous d'ajouter ce qu'il vous faut */

    // Méthodes privées ...
    // Attribut privés ...

    /**
     * @brief attente Permet d'attendre que la section critique soit libre
     **/
    PcoSemaphore attente;

    /**
     * @brief mutex Evite l'accès multiple aux variables partagées
     **/
    PcoSemaphore mutex;

    /**
     * @brief sectionOccupee Indique si la section critique est occupée
     **/
    bool sectionOccupee;

    /**
     * @brief locomotiveAttend Indique si une autre locomotive attend à l'entrée
     * de la section critique pour la libérer lorsque on sort de la section.
     **/
    bool locomotiveAttend;

public:

    /**
     * @brief SharedSection Constructeur de la classe qui représente la section partagée.
     * Initialisez vos éventuels attributs ici, sémaphores etc.
     */
    SharedSection() : attente(0), mutex(1), sectionOccupee(false), locomotiveAttend(false) {
        // TODO
    }

    /**
     * @brief access Méthode à appeler pour accéder à la section partagée, doit arrêter la
     * locomotive et mettre son thread en attente si la section est occupée par une autre locomotive.
     * Si la locomotive et son thread ont été mis en attente,
     * le thread doit être reveillé lorsque la section partagée est à nouveau libre et
     * la locomotive redémarée. (méthode à appeler un contact avant la section partagée).
     * @param loco La locomotive qui essaie accéder à la section partagée
     */
    void access(Locomotive &loco) override {
        // TODO
        mutex.acquire();

        if(sectionOccupee) {
            locomotiveAttend = true;
            loco.arreter();
            mutex.release();
            attente.acquire();
            if(!LocomotiveBehavior::emergencyStopped) {
                loco.demarrer();
            }
        }
        else {
            sectionOccupee = true;
            mutex.release();
        }

        // Exemple de message dans la console globale
        afficher_message(qPrintable(QString("The engine no. %1 accesses the shared section.").arg(loco.numero())));
    }

    /**
     * @brief leave Méthode à appeler pour indiquer que la locomotive est sortie de la section
     * partagée. (reveille les threads des locomotives potentiellement en attente).
     * @param loco La locomotive qui quitte la section partagée
     */
    void leave(Locomotive& loco) override {
        // TODO
        mutex.acquire();
        if(locomotiveAttend) {
            attente.release();
            locomotiveAttend = false;
        } else {
            sectionOccupee = false;
        }

        // Exemple de message dans la console globale
        afficher_message(qPrintable(QString("The engine no. %1 leaves the shared section.").arg(loco.numero())));

        mutex.release();
    }
};


#endif // SHAREDSECTION_H
