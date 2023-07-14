//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Rien à modifier ici

#ifndef DISPLAY_H
#define DISPLAY_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <pcosynchro/pcosemaphore.h>


class SkierItem :  public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    SkierItem();
    PcoSemaphore sem;
};

SkierItem *getSkier(unsigned int skierId);

class CableCarItem :  public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    CableCarItem();

    unsigned int arrivalStep{0};
    PcoSemaphore sem;
};

CableCarItem *getCCar();


class SlopeDisplay : public QGraphicsView
{
    Q_OBJECT
public:
    SlopeDisplay(unsigned int nbSkiers, QWidget *parent=0);
    ~SlopeDisplay();
    unsigned int m_nbPlaces;
    SkierItem *getSkier(unsigned int skierId);
    CableCarItem *getCCar();

private:
    QGraphicsScene *m_scene;
    QList<SkierItem *> m_skiers;
    CableCarItem *m_cableCar;
    unsigned waitingWidth;
    unsigned waitingLength;
    unsigned endSlopeWidth;
    unsigned endSlopeLength;
    unsigned ccSlopeGrad;

    void cCarTravel(double x, double y, unsigned int ms);


public slots:
    void finishedAnimation();
    void finishedInAnimation();
    void finishedOutAnimation();
    void finishedCCarAnimation();
    void cCarTravelUp(unsigned int ms);
    void cCarTravelDown(unsigned int ms);
    void skierTravelIn(unsigned id, unsigned ms);
    void skierTravelOut(unsigned id, unsigned int ms);
    void skierSlide(unsigned id, unsigned int ms);
    void skierWait(unsigned id, unsigned int ms);

};

#endif // DISPLAY_H
