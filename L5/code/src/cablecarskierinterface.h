//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Rien à modifier ici

#ifndef CABLECARSKIERINTERFACE_H
#define CABLECARSKIERINTERFACE_H

/*!
 * \brief La classe CableCarSkierInterface est une classe abstraite qui représente l'interface d'un
 * télécabine vu par les skieurs. Cette interface propose des méthodes pour permettre aux skieurs
 * d'intéragir avec le télécabine.
 */
class CableCarSkierInterface
{
public:

    /*!
     * \brief waitForCableCar Permet à un skieur d'attendre le télécabine (en bas à la station)
     * \param id L'id du skieur qui attend
     */
    virtual void waitForCableCar(int id) = 0;

    /*!
     * \brief waitInsideCableCar Permet à un skieur d'attendre dans le télécabine jusqu'à ce qu'il arrive en haut
     * \param id L'id du skieur qui attend
     */
    virtual void waitInsideCableCar(int id) = 0;

    /*!
     * \brief goIn Permet à un skieur de monter dans le télécabine
     * \param id L'id du skieur qui monte
     */
    virtual void goIn(int id) = 0;

    /*!
     * \brief goOut Permet à un skieur de descendre du télécabine
     * \param id L'id du skieur qui descend
     */
    virtual void goOut(int id) = 0;

    /*!
     * \brief isInService Permet de savoir si le télécabine est en service
     * \return retourn vrai si le télécabine est en service
     */
    virtual bool isInService() = 0;

    /*!
     * \brief slideSlope Permet à un skieur de descendre la piste de ski
     * Le skieur retourne ensuite vers l'arrivée du télécabine
     * \param id L'id du skieur qui descend
     */
    virtual void slideSlope(int id) = 0;

};

#endif // CABLECARSKIERINTERFACE_H
