//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Rien à modifier ici

#include "station.h"

Station::Station(PCCarInterface *interface, unsigned nbSkiers, unsigned nbCCPlaces) :
    _interface(interface), _nbSkiers(nbSkiers), _nbCCPlaces(nbCCPlaces)
{
    _stationThread = std::make_unique<PcoThread>(&Station::launchStation, this);

    logger() << "La station est ouverte !" << std::endl;
}

void Station::launchStation()
{
    // Création du télécabine
    std::shared_ptr<CableCar> cableCar = std::make_shared<PcoCableCar>(_interface, _nbCCPlaces);

    // Création des tâches pour le comportement du télécabine et des skieurs
    std::vector<std::unique_ptr<Launchable>> threads(0);

    // Création de la tâche télécabine
    threads.push_back(std::make_unique<CableCarBehavior>(cableCar));

    // Création des tâches skieurs
    for (unsigned int i = 0; i < _nbSkiers; ++i) {
        threads.push_back(std::make_unique<SkierBehavior>(_interface, cableCar));
    }

    // Mélange des tâches afin d'avoir un démarrage dans un ordre aléatoire
    std::random_shuffle(std::begin(threads), std::end(threads));

    // Démarrage des threads
    for (auto& thread : threads) {
        thread->startThread();
    }

    // Attente d'une entrée utilisateur
    QTextStream qin(stdin);
    QString Line = qin.readLine();

    // Fin de service
    cableCar->endService();

    // Attendre la fin des threads
    for (auto& thread : threads) {
        thread->join();
    }

    logger() << "The skier threads ended correctly" << std::endl;
}
