//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Auteurs : Annen Rayane, Martins Alexis

#include "pcocablecar.h"
#include <pcosynchro/pcothread.h>
#include <QRandomGenerator>
#include <QDebug>
#include <QThread>
#include <sstream>

constexpr unsigned int MIN_SECONDS_DELAY = 2;
constexpr unsigned int MAX_SECONDS_DELAY = 5;
constexpr unsigned int MIN_SECONDS_DELAY_SKIER = 1;
constexpr unsigned int MAX_SECONDS_DELAY_SKIER = 2;
constexpr unsigned int MIN_SECONDS_SKIER_SLIDE = 2;
constexpr unsigned int MAX_SECONDS_SKIER_SLIDE = 5;
constexpr unsigned int MIN_SECONDS_SKIER_GO_WAIT = 3;
constexpr unsigned int MAX_SECONDS_SKIER_GO_WAIT = 4;

constexpr unsigned int SECOND_IN_MILLISECONDS = 1000;


PcoCableCar::PcoCableCar(PCCarInterface *interface, const unsigned int capacity)
    : _interface(interface), capacity(capacity), currentSkiersInCableCar(0), isDown(false), waitingSkiersAtStation(0)
{
    // TODO
}

PcoCableCar::~PcoCableCar()
{}

void PcoCableCar::waitForCableCar(int id)
{
    // TODO
    mutex.lock();

    waitingSkiersAtStation++;

    _interface->consoleAppendText(id, "J'attend le télécabine");

    /*
     * Un skieur attend à la station si la cabine n'est pas là ou
     * qu'il n'y a plus assez de place pour lui.
     * Un skier attend forcément au moins une fois, ainsi s'il arrive d'une
     * descente et que la cabine charge déjà des gens, il ne peut pas dépasser
     */
    while((!isDown || currentSkiersInCableCar == capacity) && inService) {
        VCWaitOutside.wait(&mutex);
    }

    if(inService) {
        ++currentSkiersInCableCar;
    }

    mutex.unlock();
}

void PcoCableCar::waitInsideCableCar(int id)
{
    // TODO
    // Ajoutez ce qu'il vous semble nécessaire dans cette fonction
    mutex.lock();

    _interface->consoleAppendText(id, "J'attends dans le télécabine");

    // Attente dans le télécabine
    VCWaitInside.wait(&mutex);

    mutex.unlock();
}

void PcoCableCar::goIn(int id)
{
    // TODO
    // Ajoutez ce qu'il vous semble nécessaire dans cette fonction

    mutex.lock();

    --waitingSkiersAtStation;

    /*
     * Chaque skieur signale à la cabine qu'il est rentré, c'est ensuite
     * celle-ci qui gère le départ
     */
    VCWaitingEverybodyIn.notifyOne();

    _interface->consoleAppendText(id, "Je monte dans le télécabine");
    unsigned ms = (MIN_SECONDS_DELAY_SKIER +
                   QRandomGenerator::system()->bounded(MAX_SECONDS_DELAY_SKIER))
                   * SECOND_IN_MILLISECONDS;
    _interface->skierGoIn(id, ms);

    mutex.unlock();
}

void PcoCableCar::goOut(int id)
{
    // TODO
    // Ajoutez ce qu'il vous semble nécessaire dans cette fonction

    mutex.lock();

    _interface->consoleAppendText(id, "Je descends du télécabine");
    unsigned ms = (MIN_SECONDS_DELAY_SKIER +
                   QRandomGenerator::system()->bounded(MAX_SECONDS_DELAY_SKIER))
                   * SECOND_IN_MILLISECONDS;
    _interface->skierGoOut(id, ms);
    _interface->consoleAppendText(id, "En haut de la piste !");

    currentSkiersInCableCar--;

    /*
     * Chaque skieur signale qu'il descend de la cabine, c'est celle-ci qui
     * gère sa redescente après
     */
    VCWaitingEverybodyOut.notifyOne();

    mutex.unlock();
}


void PcoCableCar::slideSlope(int id)
{
    unsigned ms = (MIN_SECONDS_DELAY_SKIER + QRandomGenerator::system()->bounded(MAX_SECONDS_DELAY_SKIER)) * SECOND_IN_MILLISECONDS;
    QThread::msleep(ms/2);
    ms = (MIN_SECONDS_SKIER_SLIDE + QRandomGenerator::system()->bounded(MAX_SECONDS_SKIER_SLIDE)) * SECOND_IN_MILLISECONDS;
    _interface->skierSlide(id, ms);
    _interface->consoleAppendText(id, "Youhouuuu, une bonne descente !");
    ms = (MIN_SECONDS_SKIER_GO_WAIT + QRandomGenerator::system()->bounded(MAX_SECONDS_SKIER_GO_WAIT)) * SECOND_IN_MILLISECONDS;
    _interface->skierWait(id, ms);
    _interface->consoleAppendText(id, "J'attends le télécabine...");
}

bool PcoCableCar::isInService()
{
    return inService;
}

void PcoCableCar::endService()
{
    mutex.lock();
    inService = false;
    mutex.unlock();

    // Si des skieurs attendent encore, on leur force l'arrêt pour qu'ils quittent leur boucle
    VCWaitOutside.notifyAll();

    /*
     * Si la cabine attend que des skieurs montent, on la fait arrêter
     * d'attendre pour qu'elle termine sa boucle de service
     */
    VCWaitingEverybodyIn.notifyOne();
}

void PcoCableCar::goUp()
{
    _interface->consoleAppendTextCcar("Le télécabine monte");
    unsigned ms = (MIN_SECONDS_DELAY + QRandomGenerator::system()->bounded(MAX_SECONDS_DELAY + 1)) * SECOND_IN_MILLISECONDS;
    _interface->cableCarTravelUp(ms);
}

void PcoCableCar::goDown()
{
    unsigned ms = (MIN_SECONDS_DELAY + QRandomGenerator::system()->bounded(MAX_SECONDS_DELAY + 1)) * SECOND_IN_MILLISECONDS;
    _interface->consoleAppendTextCcar("Le télécabine descend");
    _interface->cableCarTravelDown(ms);
}

void PcoCableCar::loadSkiers()
{
    // TODO
    mutex.lock();

    unsigned tempWaitingSkiersAtStation = waitingSkiersAtStation;
    isDown = true;

    // On annonce aux skieurs attendant à la station qu'ils peuvent essayer de monter
    VCWaitOutside.notifyAll();

    /* Chaque skieur entrant annonce qu'il le fait, c'est alors au télécabine
     * de vérifier s'il reste encore des gens qui attendent ou s'il n'y a
     * plus de place
     */
    while (currentSkiersInCableCar < tempWaitingSkiersAtStation && currentSkiersInCableCar < capacity && inService) {

        VCWaitingEverybodyIn.wait(&mutex);
    }

    isDown = false;

    mutex.unlock();
}

void PcoCableCar::unloadSkiers()
{
    // TODO
    mutex.lock();

    // On annonce aux skieurs qu'ils peuvent descendre
    VCWaitInside.notifyAll();

    /*
     * Chaque skieur annonce qu'il descend, c'est à la cabine de vérifier qu'il
     * y ait plus personne
     */
    while (currentSkiersInCableCar) {

        VCWaitingEverybodyOut.wait(&mutex);
    }
    mutex.unlock();
}
