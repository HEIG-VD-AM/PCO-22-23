//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Rien à modifier ici

#ifndef STATION_H
#define STATION_H

#include <pcosynchro/pcothread.h>
#include <pcosynchro/pcologger.h>

#include "pcocablecar.h"
#include "skierbehavior.h"
#include "cablecarbehavior.h"
#include "pccarinterface.h"


class Station
{
public:
    Station(PCCarInterface *interface, unsigned nbSkiers, unsigned nbCCPlaces);

private:
    void launchStation();

    PCCarInterface *_interface;
    unsigned _nbSkiers;
    unsigned _nbCCPlaces;
    std::unique_ptr<PcoThread> _stationThread;
};

#endif // STATION_H
