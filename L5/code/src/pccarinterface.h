//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Rien à modifier ici

#ifndef BIKINGINTERFACE_H
#define BIKINGINTERFACE_H

#include <QObject>

#include "mainwindow.h"

/**
  \brief Classe permettant aux threads d'interagir avec la partie graphique.

  Cette classe permet d'interagir avec la partie graphique de l'application.
  Elle utilise le concept de signaux et slots afin d'envoyer les commandes
  à l'interface graphique, et peut donc être appelée par des threads, ce qui
  ne serait pas possible sinon.
  */
class PCCarInterface : public QObject
{
    Q_OBJECT

public:

    /**
      \brief Constructeur simple.

      Une seule interface peut être partagée par plusieurs threads.
      */
    PCCarInterface();

    /**
      \brief Initialisation à exécuter en début d'application.

      Fonction statique devant être appelée avant toute construction d'objet
      de type PSlideInterface.
      \param nbSkiers Nombre de skieurs
      */
    static void initialize(unsigned nbSkiers);

    /**
      \brief Fonction permettant d'afficher du texte dans une console appartenant à un skieur.
      \param consoleId Identifiant de la console. Attention, doit être compris entre 0 et nbSkiers-1.
      \param text Texte à ajouter à la console.
      */
    void consoleAppendText(unsigned int consoleId, QString text);

    /**
      \brief Fonction permettant d'afficher du texte dans la console du télécabine.
      \param text Texte à ajouter à la console.
      */
    void consoleAppendTextCcar(QString text);

    void cableCarTravelUp(unsigned int skierId, unsigned int step2, unsigned int ms);
    void cableCarTravelUp(unsigned int ms);
    void cableCarTravelDown(unsigned int ms);
    void skierGoIn(unsigned id, unsigned ms);
    void skierGoOut(unsigned id, unsigned ms);
    void skierSlide(unsigned id, unsigned ms);
    void skierWait(unsigned id, unsigned ms);

private:

    //! Indique si la fonction d'initialisation a déjà été appelée
    static bool sm_didInitialize;
    //! Fenêtre principale de l'application
    static MainWindow *mainWindow;

signals:

    void sig_consoleAppendText(unsigned int consoleId,QString text);
    void sig_consoleAppendTextCcar(QString text);
    void sig_travelUp(unsigned int ms);
    void sig_travelDown(unsigned int ms);
    void sig_skierGoIn(unsigned id, unsigned ms);
    void sig_skierGoOut(unsigned id, unsigned ms);
    void sig_skierSlide(unsigned id, unsigned ms);
    void sig_skierWait(unsigned id, unsigned ms);
};

#endif // BIKINGINTERFACE_H
