//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Rien à modifier ici

#ifndef LAUNCHABLE_H
#define LAUNCHABLE_H

#include <QDebug>

#include <pcosynchro/pcothread.h>

/*!
 * \brief La classe Launchable est une classe abstraite qui représente le fait d'avoir un thread
 * associé qui permet d'être lancé, thread qui exécute la fonction run() qui représente le
 * comportement de la classe qui la définit.
 */
class Launchable
{
public:
    Launchable() {}

    /*!
     * \brief Lance un thread avec la fonction run()
     */
    void startThread() {
        if (thread == nullptr) {
            printStartMessage();
            #ifdef __USE_STD_THREADS__
                    thread = std::make_unique<std::thread>(&Launchable::run, this);
            #else
                    thread = std::make_unique<PcoThread>(&Launchable::run, this);
            #endif
        }
    }

    /*!
     * \brief Attend la fin du thread lancé
     */
    void join() {
        if (thread != nullptr) {
            thread->join();
            printCompletionMessage();
        }
    };

protected:

    /*!
     * \brief La fonction à lancer, ici abstraite (virtuelle pure), est redéfinie par les
     * classes concrètes qui héritent de la classe Launchable.
     */
    virtual void run() = 0;

    /*!
     * \brief Message affiché au lancement du thread
     */
    virtual void printStartMessage() {qDebug() << "[START] Un thread lancé";}

    /*!
     * \brief Message affiché après la fin du thread
     */
    virtual void printCompletionMessage() {qDebug() << "[STOP] Un thread a terminé";}

    /*!
     * \brief Le thread associé
     */
#ifdef __USE_STD_THREADS__
    std::unique_ptr<std::thread> thread = nullptr;
#else
    std::unique_ptr<PcoThread> thread = nullptr;
#endif


};

#endif // LAUNCHABLE_H
