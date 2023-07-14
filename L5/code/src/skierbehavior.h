//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Rien à modifier ici

#ifndef SKIERBEHAVIOR_H
#define SKIERBEHAVIOR_H

#include "launchable.h"
#include "cablecar.h"
#include "pccarinterface.h"

/*!
 * \brief La classe SkierBehavior est une classe abstraite qui représente le comportement d'un skieur
 */
class SkierBehavior : public Launchable
{
private:
    PCCarInterface *_interface;

public:

    /*!
     * \brief Constructeur de la classe PcoSkier, permet d'instancier un skieur
     * \param cableCar Pointeur sur le télécabine que le skieur utilise pour monter en haut de la montagne
     */
    SkierBehavior(PCCarInterface *interface, std::shared_ptr<CableCarSkierInterface> cableCar);

    /*!
     * \brief Fonction lancée par le thread, représente le comportement du skieur
     */
    void run() override;

    /*!
     * \brief Message affiché lors du démarrage du thread
     */
    void printStartMessage() override {qDebug() << "[START] Thread du skieur" << skierId << "lancé";}

    /*!
     * \brief Message affiché lorsque le thread a terminé
     */
    void printCompletionMessage() override {qDebug() << "[STOP] Thread du skieur" << skierId << "a terminé correctement";}

    /*!
     * \brief L'identificateur du skieur
     */
    const int skierId;

    /*!
     * \brief L'identificateur pour le prochain Skieur créé
     */
    static int nextId;

    /*!
     * \brief Le télécabine utilisée par le skieur
     */
    std::shared_ptr<CableCarSkierInterface> cableCar;


};

#endif // SKIERBEHAVIOR_H
