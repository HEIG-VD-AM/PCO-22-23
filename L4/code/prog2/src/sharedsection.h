//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/


#ifndef SHAREDSECTION_H
#define SHAREDSECTION_H

#include <QDebug>

#include <pcosynchro/pcosemaphore.h>

#include "locomotive.h"
#include "ctrain_handler.h"
#include "sharedsectioninterface.h"
#include "locomotivebehavior.h"

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

    /**
     * @brief prioriteActuelle Indique la valeur de la priorité actuelle qu'il est
     * nécessaire d'avoir pour entrer en section critique.
     */
    int prioriteActuelle;

    /**
     * @brief modePrio Indique quel mode de priorité est choisi actuellement
     */
    PriorityMode modePrio;

public:

    /**
     * @brief SharedSection Constructeur de la classe qui représente la section partagée.
     * Initialisez vos éventuels attributs ici, sémaphores etc.
     */
    SharedSection() : attente(0), mutex(1), sectionOccupee(false),
                      locomotiveAttend(false), prioriteActuelle(-1),
                      modePrio(PriorityMode::LOW_PRIORITY) {
        // TODO
    }

    /**
     * @brief request Méthode a appeler pour indiquer que la locomotive désire accéder à la
     * section partagée (deux contacts avant la section partagée).
     * @param loco La locomotive qui désire accéder
     * @param priority Le niveau de priorité de la locomotive
     */
    void request(Locomotive& loco, int priority) override {
        // TODO

        mutex.acquire();

        /* On vérifie si la priorité doit être changée ou car celle-ci est plus
         * haute ou plus basse que l'actuelle.
         * On vérifie aussi la valeur -1, car c'est la valeur par défaut
         * si aucune priorité n'est encore affectée.
         */
        if(prioriteActuelle == -1 ||
           (modePrio == PriorityMode::HIGH_PRIORITY && priority > prioriteActuelle)
           || (modePrio == PriorityMode::LOW_PRIORITY && priority < prioriteActuelle)) {
            prioriteActuelle = priority;
        }

        mutex.release();

        // Exemple de message dans la console globale
        afficher_message(qPrintable(QString("The engine no. %1 requested the shared section.").arg(loco.numero())));
    }

    /**
     * @brief getAccess Méthode à appeler pour accéder à la section partagée, doit arrêter la
     * locomotive et mettre son thread en attente si la section est occupée ou va être occupée
     * par une locomotive de plus haute priorité. Si la locomotive et son thread ont été mis en
     * attente, le thread doit être reveillé lorsque la section partagée est à nouveau libre et
     * la locomotive redémarée. (méthode à appeler un contact avant la section partagée).
     * @param loco La locomotive qui essaie accéder à la section partagée
     * @param priority Le niveau de priorité de la locomotive
     */
    void access(Locomotive &loco, int priority) override {
        // TODO
        mutex.acquire();

        /* Si un train est déjà dans la section ou que notre priorité ne correspond
         *  pas à celle nécessaire pour rentrer dans la section, nous devons attendre.
         */
        if(sectionOccupee ||  prioriteActuelle != priority) {
            locomotiveAttend = true;
            loco.arreter();
            mutex.release();
            attente.acquire();

            /* Permet d'éviter le cas où un train sort de la section critique
             * grâce à l'inertie et que l'arrêt d'urgence est pressé au même moment.
             */
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

        /* On vérifie si la seconde locomotive est en attente.
         *  Dans ce cas, on lui signale qu'elle peut se relancer.
         *  Sinon on quitte simplement la section et on reset la priorité.
         */
        if(locomotiveAttend) {
            attente.release();
            locomotiveAttend = false;
        } else {
            sectionOccupee = false;
            prioriteActuelle = -1;
        }

        // Exemple de message dans la console globale
        afficher_message(qPrintable(QString("The engine no. %1 leaves the shared section.").arg(loco.numero())));

        mutex.release();
    }


    void togglePriorityMode() override {
        modePrio = (modePrio == PriorityMode::HIGH_PRIORITY ?
                    PriorityMode::LOW_PRIORITY : PriorityMode::HIGH_PRIORITY);
        /* TODO */
    }

private:

    /* A vous d'ajouter ce qu'il vous faut */

    // Méthodes privées ...
    // Attributes privés ...
};


#endif // SHAREDSECTION_H
