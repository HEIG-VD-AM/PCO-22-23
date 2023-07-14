//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Auteurs : Annen Rayane, Martins Alexis

#ifndef PCOCABLECAR_H
#define PCOCABLECAR_H

#include "cablecar.h"
#include "pccarinterface.h"
#include "pcosynchro/pcoconditionvariable.h"
#include "pcosynchro/pcomutex.h"

/*!
 * \brief La classe PcoCableCar implémente la classe abstraite CableCar, la classe PcoCableCar
 * doit fournir les méthodes pour modéliser le comportement du télécabine ainsi que les méthodes qui
 * permettent aux skieurs d'intéragir avec le télécabine.
 */
class PcoCableCar : public CableCar
{   
public:

    /*!
     * \brief constructeur qui permet de définir la capaicité du télécabine
     * \param capacity La capacité (nombre de places) dans le télécabine
     * Par défaut la capacité est de 10.
     */
    PcoCableCar(PCCarInterface *interface, const unsigned int capacity = 10);

    /*!
     * \brief destructeur de la classe PcoCableCar
     */
    ~PcoCableCar();

    /********************************************
     * Méthodes de l'interface pour les skieurs *
     *******************************************/

    /*!
     * \brief Permet à un skieur d'attendre le télécabine (en bas à la station)
     * \param id L'id du skieur qui attend
     */
    void waitForCableCar(int id) override;

    /*!
     * \brief Permet à un skieur d'attendre dans le télécabine jusqu'à ce qu'il arrive en haut
     * \param id L'id du skieur qui attend
     */
    void waitInsideCableCar(int id) override;

    /*!
     * \brief Permet à un skieur de monter dans le télécabine
     * \param id L'id du skieur qui monte
     */
    void goIn(int id) override;

    /*!
     * \brief Permet à un skieur de descendre du télécabine
     * \param id L'id du skieur qui descend
     */
    void goOut(int id) override;

    /*!
     * \brief slideSlope Permet à un skieur de descendre la piste de ski
     * Le skieur retourne ensuite vers l'arrivée du télécabine
     * \param id L'id du skieur qui descend
     */
    void slideSlope(int id) override;

    /****************************************************
     * Méthodes pour l'interface du télécabine lui-même *
     ***************************************************/

    /*!
     * \brief Représente le fait que le télécabine monte
     */
    void goUp() override;

    /*!
     * \brief Représente le fait que le télécabine descend
     */
    void goDown() override;

    /*!
     * \brief Représente le fait que le télécabine charge des skieurs
     */
    void loadSkiers() override;

    /*!
     * \brief Représente le fait que le télécabine décharge des skieurs
     */
    void unloadSkiers() override;

    /**********************
     * Méthodes générales *
     *********************/

    /*!
     * \brief Permet de savoir si le télécabine est en service
     * \return retourn vrai si le télécabine est en service
     */
    bool isInService() override;

    /*!
     * \brief Permet de mettre fin au service du télécabine
     */
    void endService() override;

protected:
    PCCarInterface *_interface;

    /*!
     * \brief La capacité du télécabine en nombre de skieurs (nombre de places)
     */
    const unsigned int capacity;

    /*!
     * \brief L'état du service du télécabine
     */
    bool inService = true;

    /*!
     * \brief VCWaitingEverybodyOut Attente que tout le monde soit sorti de la
     * télécabine
     */
    PcoConditionVariable VCWaitingEverybodyOut;

    /*!
     * \brief VCWaitingEverybodyIn Attente que tout le monde qui attend soit
     * rentré ou qu'il n'y ait plus de place
     */
    PcoConditionVariable VCWaitingEverybodyIn;

    /*!
     * \brief VCWaitInside Attente que la télécabine arrive en haut pour
     * descendre
     */
    PcoConditionVariable VCWaitInside;

    /*!
     * \brief VCWaitOutside Attente à la station que la cabine soit là
     */
    PcoConditionVariable VCWaitOutside;

    /*!
     * \brief mutex Mutex utilisé pour le moniteur de Mesa
     */
    PcoMutex mutex;

    /*!
     * \brief currentSkiersInCableCar Nombre de skiers en cabine
     */
    unsigned int currentSkiersInCableCar;

    /*!
     * \brief waitingSkiersAtStation Nombre de skier qui attendent à la station
     */
    unsigned int waitingSkiersAtStation;

    /*!
     * \brief isDown Indique lorsque la cabine est en-bas de la station
     */
    bool isDown;

    // TODO: A vous d'ajouter les attributs dont vous avez besoin

};

#endif // PCOCABLECAR_H
