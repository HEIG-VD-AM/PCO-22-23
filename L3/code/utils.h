/**
* @brief Déclaration de la class Utils, vous pouvez ajoutez des choses, mais ne rien changer.
* @file utils.h
* @author Hugo Ducommun
* @author Alexis Martins
*/

#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <QRandomGenerator>
#include <iostream>
#include <pcosynchro/pcothread.h>
#include <QDebug>
#include <QTextStream>

#include "mine.h"
#include "factory.h"
#include "wholesale.h"
#include "seller.h"

#define NB_MINES 3
#define NB_FACTORIES 2
#define NB_WHOLESALER 4
#define MINES_FUND 200
#define FACTORIES_FUND 300
#define WHOLESALERS_FUND 150

std::vector<Mine*> create_mines(int nbMines, int id_start);
std::vector<Factory*> create_factories(int nbFactories, int id_start);
std::vector<Wholesale*> create_wholesaler(int nbWholesaler, int id_start);

class Utils {
private:
    std::vector<Mine*> mines;
    std::vector<Factory*> factories;
    std::vector<Wholesale*> wholesalers;

    std::vector<std::unique_ptr<PcoThread>> threads;
    std::unique_ptr<PcoThread> utilsThread;

    void endService();

    void run();
public:
    Utils(int nb_mines, int nb_factory, int nb_wholesale);


protected:
    bool eventFilter(QObject* obj, QEvent* event);

};

#endif // UTILS_H
