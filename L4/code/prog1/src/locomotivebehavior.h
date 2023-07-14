//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Auteurs : Alexis Martins, Anthony David, Pablo Saez

#ifndef LOCOMOTIVEBEHAVIOR_H
#define LOCOMOTIVEBEHAVIOR_H

#include "locomotive.h"
#include "launchable.h"
#include "sharedsectioninterface.h"
#include "sharedstation.h"

/**
 * @brief La classe LocomotiveBehavior représente le comportement d'une locomotive
 */
class LocomotiveBehavior : public Launchable
{
public:
    /*!
     * \brief locomotiveBehavior Constructeur de la classe
     * \param loco la locomotive dont on représente le comportement
     */
    static bool emergencyStopped;
    LocomotiveBehavior(Locomotive& loco,
                       std::shared_ptr<SharedSectionInterface> sectionPartagee,
                       int pointDeContact, int tours,
                       std::vector<std::pair<int,int>> aiguillage,
                       std::vector<int> pointsControleAiguillage,
                       std::vector<int> pointsControlePartages,
                       SharedStation& gare)
                       : loco(loco), sectionPartagee(sectionPartagee),
                         pointDeContact(pointDeContact), toursAvantInverse(tours),
                         aiguillage(aiguillage),
                         pointsControleAiguillage(pointsControleAiguillage),
                         pointsControleAiguillagePartages(pointsControlePartages), gare(gare)
    {}

protected:
    /*!
     * \brief run Fonction lancée par le thread, représente le comportement de la locomotive
     */
    void run() override;

    /*!
     * \brief printStartMessage Message affiché lors du démarrage du thread
     */
    void printStartMessage() override;

    /*!
     * \brief printCompletionMessage Message affiché lorsque le thread a terminé
     */
    void printCompletionMessage() override;

    /**
     * @brief loco La locomotive dont on représente le comportement
     */
    Locomotive& loco;

    /**
     * @brief sharedSection Pointeur sur la section partagée
     */
    std::shared_ptr<SharedSectionInterface> sectionPartagee;

    /**
     * @brief pointDeContact Défini le point pour compter les tours
     */
    int pointDeContact;

    /**
     * @brief toursAvantInverse Tours à faire pour inverse la loco
     **/
    int toursAvantInverse;

    /**
     * @brief aiguillage Aiguillage de la section partagée et l'état qu'ils
     * doivent avoir pour le passage de la loco
     **/
    std::vector<std::pair<int,int>> aiguillage;

    /**
     * @brief pointsControleAiguillage Points de contrôle des aiguillages de
     * chaque locomotive. Lorsque la locomotive est sur ce point, elle change
     * l'état de l'aiguillage
     **/
    std::vector<int> pointsControleAiguillage;

    /**
     * @brief pointsControleAiguillagePartages Comme pour les points de
     * contrôle de l'aiguillage, sauf que ceux-ci sont dans la section
     * critique et donc partagés entre les locomotives.
     **/
    std::vector<int> pointsControleAiguillagePartages;

    /**
     * @brief gare Gare de la locomotive
     **/
    SharedStation& gare;

    /*
     * Vous êtes libres d'ajouter des méthodes ou attributs
     *
     * Par exemple la priorité ou le parcours
     */
};

#endif // LOCOMOTIVEBEHAVIOR_H
