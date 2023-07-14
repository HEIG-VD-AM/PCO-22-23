//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Rien à modifier ici

#ifndef CABLECAR_H
#define CABLECAR_H

#include "cablecarskierinterface.h"
#include "cablecarinterface.h"

/*!
 * \brief La classe CableCar est une classe abstraite qui représente un télécabine
 */
class CableCar : public CableCarInterface, public CableCarSkierInterface
{
public:

    /*!
     * \brief Permet de mettre fin au service du télécabine
     */
    virtual void endService() = 0;
};

#endif // CABLECAR_H
