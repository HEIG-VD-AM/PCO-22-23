//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Rien à modifier ici en dehors des valeurs NB_SKIERS et CABLE_CAR_CAPACITY pour vos essais
// Vous pouvez aussi changer l'ordre de lancement des threads ou ajouter des délais aléatoires
// avant vos lancements etc. pour effectuer des tests.
//
// Modifications : -
// Auteurs : Annen Rayane, Martins Alexis

#include <QDebug>

#include <QApplication>
#include <QCommandLineParser>

#include <vector>
#include <algorithm>

#include <pcosynchro/pcothread.h>
#include <pcosynchro/pcologger.h>


#include "station.h"
#include "pccarinterface.h"


constexpr unsigned int NB_SKIERS = 8;
constexpr unsigned int CABLE_CAR_CAPACITY = 3;

int main(int argc, char *argv[])
{
    logger().setVerbosity(1);

    QApplication a(argc, argv);
    QCommandLineParser parser;
    QCommandLineOption cable_car_capa_nb(QStringList() << "s" << "steps",
                                  QCoreApplication::translate("main",
                                       "Number or steps on toboggan, "
                                       "the slider is considered as a step, minimum 2"),
                                  QCoreApplication::translate("main", "steps"));

    QCommandLineOption nb_skiers_nb(QStringList() << "k" << "kids",
                                 QCoreApplication::translate("main",
                                                             "Number of kids on the toboggan, minimum 1"),
                                 QCoreApplication::translate("main", "kids"));

    parser.addOption(cable_car_capa_nb);
    parser.addOption(nb_skiers_nb);
    parser.process(a);

    int cable_car_capa = parser.value(cable_car_capa_nb).toInt();
    int nb_skiers = parser.value(nb_skiers_nb).toInt();

     logger() << "Cable car capacity from cmd line: " << cable_car_capa << std::endl;
     logger() << "Number of skiers from cmd line: " << nb_skiers << std::endl;

    if (nb_skiers < 1) {
        nb_skiers = NB_SKIERS;
        logger() << "Number of skiers passed in commande line argument invalid."
                     " Using default value:"<< nb_skiers << std::endl;
    }

    if (cable_car_capa < 2) {
        cable_car_capa = CABLE_CAR_CAPACITY;
        logger() << "Cable car capacity passed in commande line argument invalid."
                     " Using default value:"<< cable_car_capa << std::endl;
    }


    qDebug() << "Démarrage de la simulation de skieurs et télécabine ...";
    qDebug() << "Nombre de skieurs :" << nb_skiers;
    qDebug() << "Capacité du télécabine :" << cable_car_capa;
    qDebug() << "Appuyez sur ENTER pour terminer\n";


    // Initialisation de la partie graphique de l'application
    PCCarInterface::initialize(nb_skiers);

    // Création de l'interface pour les commandes à la partie graphique
    auto guiInterface = new PCCarInterface();

    Station station(guiInterface, nb_skiers, cable_car_capa);

    return a.exec();
}
