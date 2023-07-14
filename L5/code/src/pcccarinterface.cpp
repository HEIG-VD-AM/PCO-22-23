//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Rien à modifier ici

#include <iostream>

#include <QMessageBox>
#include <QThread>
#include "pccarinterface.h"
#include "display.h"

using namespace std;


bool PCCarInterface::sm_didInitialize   = false;
MainWindow *PCCarInterface::mainWindow  = nullptr;

PCCarInterface::PCCarInterface()
{
    if (!sm_didInitialize) {
        cout << "Vous devez appeler PSlideInterface::initialize()" << endl;
        QMessageBox::warning(nullptr,"Erreur","Vous devez appeler "
                             "PSlideInterface::initialize() avant de créer un "
                             "objet PSlideInterface");
        exit(-1);
    }

    if (!QObject::connect(this,
                          SIGNAL(sig_consoleAppendText(unsigned int,QString)),
                          mainWindow,
                          SLOT(consoleAppendText(unsigned int,QString)),Qt::QueuedConnection)) {
        std::cout << "Error with signal-slot connection" << std::endl;
    }

    if (!QObject::connect(this,
                          SIGNAL(sig_consoleAppendTextCcar(QString)),
                          mainWindow,
                          SLOT(consoleAppendTextCcar(QString)), Qt::QueuedConnection)) {
        std::cout << "Error with signal-slot connection" << std::endl;
    }

    if (!QObject::connect(this,
                          SIGNAL(sig_travelUp(uint)),
                          mainWindow,
                          SLOT(cCarTravelUp(unsigned int)),Qt::QueuedConnection)) {
        std::cout << "Error with signal-slot connection" << std::endl;
    }

    if (!QObject::connect(this,
                          SIGNAL(sig_travelDown(uint)),
                          mainWindow,
                          SLOT(cCarTravelDown(unsigned int)),Qt::QueuedConnection)) {
        std::cout << "Error with signal-slot connection" << std::endl;
    }

    if (!QObject::connect(this,
                          SIGNAL(sig_skierGoIn(unsigned, unsigned)),
                          mainWindow,
                          SLOT(skierTravelIn(unsigned int, unsigned int)),Qt::QueuedConnection)) {
        std::cout << "Error with signal-slot connection" << std::endl;
    }

    if (!QObject::connect(this,
                          SIGNAL(sig_skierGoOut(unsigned, unsigned)),
                          mainWindow,
                          SLOT(skierTravelOut(unsigned int, unsigned int)),Qt::QueuedConnection)) {
        std::cout << "Error with signal-slot connection" << std::endl;
    }

    if (!QObject::connect(this,
                          SIGNAL(sig_skierSlide(unsigned, unsigned)),
                          mainWindow,
                          SLOT(skierSlide(unsigned int, unsigned int)),Qt::QueuedConnection)) {
        std::cout << "Error with signal-slot connection" << std::endl;
    }

    if (!QObject::connect(this,
                          SIGNAL(sig_skierWait(unsigned, unsigned)),
                          mainWindow,
                          SLOT(skierWait(unsigned int, unsigned int)),Qt::QueuedConnection)) {
        std::cout << "Error with signal-slot connection" << std::endl;
    }
}

#include <QTest>

void PCCarInterface::cableCarTravelUp(unsigned int ms)
{
    emit sig_travelUp(ms);
    getCCar()->sem.acquire();
}

void PCCarInterface::cableCarTravelDown(unsigned int ms)
{
    emit sig_travelDown(ms);
    getCCar()->sem.acquire();
}


void PCCarInterface::consoleAppendText(unsigned int consoleId, QString text)
{
    emit sig_consoleAppendText(consoleId, std::move(text));
}

void PCCarInterface::consoleAppendTextCcar(QString text)
{
    emit sig_consoleAppendTextCcar(std::move(text));
}

void PCCarInterface::skierGoIn(unsigned id, unsigned ms)
{
    emit sig_skierGoIn(id, ms);
    getSkier(id)->sem.acquire();

}

void PCCarInterface::skierGoOut(unsigned id, unsigned ms)
{
    emit sig_skierGoOut(id, ms);
    getSkier(id)->sem.acquire();
}

void PCCarInterface::skierSlide(unsigned id, unsigned ms)
{
    emit sig_skierSlide(id, ms);
    getSkier(id)->sem.acquire();
}

void PCCarInterface::skierWait(unsigned id, unsigned ms)
{
    emit sig_skierWait(id, ms);
    getSkier(id)->sem.acquire();
}


void PCCarInterface::initialize(unsigned nbSkiers)
{
    if (sm_didInitialize) {
        cout << "Vous devez ne devriez appeler PSlideInterface::initialize()"
             << " qu'une seule fois" << endl;
        QMessageBox::warning(nullptr,"Erreur","Vous ne devriez appeler "
                             "PSlideInterface::initialize() "
                             "qu'une seule fois");
        return;
    }

    mainWindow = new MainWindow(nbSkiers, nullptr);
    mainWindow->show();
    sm_didInitialize=true;
}
