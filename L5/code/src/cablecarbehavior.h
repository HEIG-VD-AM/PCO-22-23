//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Rien à modifier ici

#ifndef CABLECARBEHAVIOR_H
#define CABLECARBEHAVIOR_H

#include "launchable.h"
#include "cablecarinterface.h"

/*!
 * \brief La classe CableCarBehavior permet de représenter le comportement de la tâche télécabine
 */
class CableCarBehavior : public Launchable
{
public:
    /*!
     * \brief Constructeur
     * \param cableCar le télécabine dont on modélise le comportement
     */
    CableCarBehavior(std::shared_ptr<CableCarInterface> cableCar) : cableCar(cableCar) {}


    /*!
     * \brief Fonction lancée par le thread, représente le comportement du télécabine
     */
    void run() override;

protected:

    /*!
     * \brief Message affiché au lancement du thread
     */
    void printStartMessage() override {qDebug() << "[START] Thread du télécabine lancé";}

    /*!
     * \brief Message affiché après la fin du thread
     */
    void printCompletionMessage() override {qDebug() << "[STOP] Thread du télécabine a terminé correctement";}

    /*!
     * \brief Le télécabine dont on modélise le comportement
     */
    const std::shared_ptr<CableCarInterface> cableCar;
};

#endif // CABLECARBEHAVIOR_H
