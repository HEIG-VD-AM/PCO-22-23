//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Rien à modifier ici

#include <cmath>


#include <QPaintEvent>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QEventLoop>
#include <QRandomGenerator>

#include <pcosynchro/pcomutex.h>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include "display.h"

constexpr double SCENEWIDTH = 300.0;
constexpr double SCENELENGTH = 600.0;
constexpr double SCENEOFFSET = 100.0;
constexpr double SKIERWIDTH = 30.0;
constexpr double PILONEWIDTH = 42.0;
constexpr double PILONEHEIGHT = 96.0;
constexpr double STATIONWIDTH = 229.0;
constexpr double STATIONHEIGHT = 72.0;
constexpr double CCWIDTH = 70.0;
constexpr int NBKIDICONS = 30;


static SlopeDisplay* theDisplay;

SkierItem::SkierItem() = default;
CableCarItem::CableCarItem() = default;

SkierItem *getSkier(unsigned int id)
{
    return theDisplay->getSkier(id);
}

CableCarItem *getCCar()
{
    return theDisplay->getCCar();
}

SlopeDisplay::SlopeDisplay(unsigned int nbSkiers, QWidget *parent):
    QGraphicsView(parent)
{

    theDisplay = this;

    m_scene=new QGraphicsScene(this);
    this->setRenderHints(QPainter::Antialiasing |
                         QPainter::SmoothPixmapTransform);
    this->setMinimumHeight(SCENEOFFSET+SCENEWIDTH);
    this->setMinimumWidth(SCENEOFFSET+SCENELENGTH);
    this->setScene(m_scene);

    QPen pen;
    QBrush brush(QColor(100,100,100));

    QGraphicsPixmapItem *pm = m_scene->addPixmap( QPixmap(":images/station_ski.png") );
    pm->setPos(SCENEOFFSET, SCENEOFFSET - 100);

    waitingWidth = SCENEWIDTH/7.;
    waitingLength = SCENELENGTH/3.;
    endSlopeWidth = SCENEWIDTH/7.;
    endSlopeLength = SCENELENGTH/3. * 2;

    pen.setWidthF(2);

    QPolygonF pathCc;
    ccSlopeGrad = SCENELENGTH/3;
    pathCc << QPointF(SCENEOFFSET + waitingLength/2., SCENEOFFSET + SCENEWIDTH - waitingWidth - CCWIDTH);
    pathCc << QPointF(SCENEOFFSET + waitingLength/2. + ccSlopeGrad, SCENEOFFSET);

    QGraphicsPixmapItem *pm_pil = m_scene->addPixmap( QPixmap(":images/pilone.png") );
    pm_pil->setPos(SCENEOFFSET + waitingLength/2. - PILONEWIDTH/2., SCENEOFFSET + SCENEWIDTH - waitingWidth - CCWIDTH);

    QGraphicsPixmapItem *pm_pil2 = m_scene->addPixmap( QPixmap(":images/pilone.png") );
    pm_pil2->setPos(SCENEOFFSET + waitingLength/2. - PILONEWIDTH/2. + ccSlopeGrad/2., SCENEOFFSET + SCENEWIDTH/2. - waitingWidth/2. - CCWIDTH/2.);

    QGraphicsPixmapItem *pm_pil3 = m_scene->addPixmap( QPixmap(":images/pilone.png") );
    pm_pil3->setPos(SCENEOFFSET + waitingLength/2. - PILONEWIDTH/2. + ccSlopeGrad, SCENEOFFSET);

    QGraphicsPixmapItem *pm_station = m_scene->addPixmap( QPixmap(":images/station_ccar.png") );
    pm_station->setPos(SCENEOFFSET, SCENEOFFSET + SCENEWIDTH - STATIONHEIGHT);

    m_scene->addPolygon(pathCc, pen, brush);

    QPixmap ccImg(QString(":images/telecabine.png"));
    QPixmap ccPixmap;
    ccPixmap = ccImg.scaledToWidth(CCWIDTH);
    m_cableCar = new CableCarItem();
    m_cableCar->setPixmap(ccPixmap);
    m_scene->addItem(m_cableCar);
    double x = SCENEOFFSET + waitingLength/2. - CCWIDTH/2.;
    double y = SCENEOFFSET + SCENEWIDTH - waitingWidth - CCWIDTH;
    m_cableCar->setPos(x, y);

    for (unsigned int i = 0; i < nbSkiers; i++)
    {
        QPixmap img(QString(":images/32x32/p%1.png").arg(m_skiers.size() % NBKIDICONS));
        QPixmap kidPixmap;
        kidPixmap=img.scaledToWidth(SKIERWIDTH);
        auto skier = new SkierItem();
        skier->setPixmap(kidPixmap);
        m_scene->addItem(skier);
        m_skiers.append(skier);
        auto r = static_cast<unsigned>(rand());
        unsigned x = SCENEOFFSET + r % waitingLength - SKIERWIDTH;
        unsigned y = SCENEOFFSET + SCENEWIDTH - (r % waitingWidth) - SKIERWIDTH;
        skier->setPos(x,y);
        //skier->hide();
    }
}

SlopeDisplay::~SlopeDisplay()
{
    while (!m_skiers.empty()) {
        SkierItem *p = m_skiers.at(0);
        p->sem.release();
        m_skiers.removeFirst();
        delete p;
    }
}


SkierItem *SlopeDisplay::getSkier(unsigned int skierId)
{
    return m_skiers.at(skierId);
}

CableCarItem *SlopeDisplay::getCCar()
{
    return m_cableCar;
}

void SlopeDisplay::cCarTravelUp(unsigned int ms)
{
    cCarTravel(SCENEOFFSET + waitingLength/2. + ccSlopeGrad - CCWIDTH/2., SCENEOFFSET, ms);
}

void SlopeDisplay::cCarTravelDown(unsigned int ms)
{
    cCarTravel(SCENEOFFSET + waitingLength/2. - CCWIDTH/2., SCENEOFFSET + SCENEWIDTH - waitingWidth - CCWIDTH, ms);
}

void SlopeDisplay::cCarTravel(double x, double y, unsigned int ms)
{
    static PcoMutex mutex;
    mutex.lock();
    auto group = new QParallelAnimationGroup(this);
    {
        m_cableCar->show();
        auto animation = new QPropertyAnimation(m_cableCar, "pos");
        animation->setDuration(static_cast<int>(ms)-10);
        animation->setEndValue(QPoint(x, y));
        group->addAnimation(animation);
    }

    group->start();
    QObject::connect(group, SIGNAL(finished()), this, SLOT(finishedCCarAnimation()));
    mutex.unlock();
}

void SlopeDisplay::skierTravelIn(unsigned id, unsigned int ms)
{
    double x = SCENEOFFSET + waitingLength/2. - SKIERWIDTH/2.;
    double y = SCENEOFFSET + SCENEWIDTH - waitingWidth - CCWIDTH/2.;

    static PcoMutex skiMutexIn;
    skiMutexIn.lock();

    auto group = new QParallelAnimationGroup(this);

    {
        SkierItem *skier = getSkier(id);
        skier->show();
        auto animation = new QPropertyAnimation(skier, "pos");
        animation->setDuration(static_cast<unsigned>(ms) - 10);
        animation->setEndValue(QPoint(x, y));
        group->addAnimation(animation);
    }

    group->start();
    QObject::connect(group, SIGNAL(finished()), this, SLOT(finishedInAnimation()));
    skiMutexIn.unlock();
}

void SlopeDisplay::skierTravelOut(unsigned id, unsigned int ms)
{
    double start = SCENEOFFSET + waitingLength/2. + SCENELENGTH/3. + CCWIDTH;
    double end = SCENEOFFSET + waitingLength/2. + SCENELENGTH/3. + SCENELENGTH/2.;

    double x = start + QRandomGenerator::system()->bounded(end - start) - SKIERWIDTH;
    double y = SCENEOFFSET + CCWIDTH - SKIERWIDTH;
    double start_pos_x = SCENEOFFSET + waitingLength/2. + ccSlopeGrad - SKIERWIDTH/2. ;
    double start_pos_y = SCENEOFFSET + CCWIDTH/2.;

    static PcoMutex skiMutexOut;
    skiMutexOut.lock();

    auto group = new QParallelAnimationGroup(this);

    {
        SkierItem *skier = getSkier(id);
        auto animation = new QPropertyAnimation(skier, "pos");
        animation->setStartValue(QPoint(start_pos_x, start_pos_y));
        skier->show();
        animation->setDuration(static_cast<int>(ms)-10);
        animation->setEndValue(QPoint(x, y));
        group->addAnimation(animation);
    }

    group->start();
    QObject::connect(group, SIGNAL(finished()), this, SLOT(finishedOutAnimation()));
    skiMutexOut.unlock();
}

void SlopeDisplay::skierSlide(unsigned id, unsigned int ms)
{
    double start = SCENEOFFSET + waitingLength + endSlopeLength/4;
    double end = SCENEOFFSET + endSlopeLength + waitingLength;
    double x = start + QRandomGenerator::system()->bounded(end - start - endSlopeLength/4) - SKIERWIDTH;
    double y = SCENEOFFSET + SCENEWIDTH - endSlopeWidth;

    static PcoMutex slopeMutex;

    slopeMutex.lock();

    auto group = new QParallelAnimationGroup(this);

    {
        SkierItem *skier = getSkier(id);
        auto animation = new QPropertyAnimation(skier, "pos");
        animation->setDuration(static_cast<int>(ms) - 10);
        animation->setEndValue(QPoint(x, y));
        group->addAnimation(animation);
    }

    group->start();
    QObject::connect(group, SIGNAL(finished()), this, SLOT(finishedAnimation()));
    slopeMutex.unlock();
}

void SlopeDisplay::skierWait(unsigned id, unsigned int ms)
{
    auto r = static_cast<unsigned>(rand());
    unsigned x = SCENEOFFSET + (r % waitingLength/2 - SKIERWIDTH) + SKIERWIDTH;
    unsigned y = SCENEOFFSET + SCENEWIDTH - (r % waitingWidth/2 + SKIERWIDTH);

    static PcoMutex waitMutex;

    waitMutex.lock();

    auto group = new QParallelAnimationGroup(this);

    {
        SkierItem *skier = getSkier(id);
        auto animation = new QPropertyAnimation(skier, "pos");
        animation->setDuration(static_cast<int>(ms) - 10);
        animation->setEndValue(QPoint(x, y));
        group->addAnimation(animation);
    }

    group->start();
    QObject::connect(group, SIGNAL(finished()), this, SLOT(finishedAnimation()));
    waitMutex.unlock();
}

void SlopeDisplay::finishedAnimation()
{
    auto group = dynamic_cast<QAnimationGroup*>(sender());
    for(int i = 0; i<group->animationCount(); i++) {
        auto animation = dynamic_cast<QPropertyAnimation*>(group->animationAt(i));
        auto skier = dynamic_cast<SkierItem*>(animation->targetObject());
        if (skier) {
            skier->sem.release();
        }
    }
    delete group;
}

void SlopeDisplay::finishedInAnimation()
{
    auto group = dynamic_cast<QAnimationGroup*>(sender());
    for(int i = 0; i<group->animationCount(); i++) {
        auto animation = dynamic_cast<QPropertyAnimation*>(group->animationAt(i));
        auto skier = dynamic_cast<SkierItem*>(animation->targetObject());
        if (skier) {
            skier->hide();
            skier->sem.release();
        }
    }
    delete group;
}

void SlopeDisplay::finishedOutAnimation()
{
    auto group = dynamic_cast<QAnimationGroup*>(sender());
    for(int i = 0; i<group->animationCount(); i++) {
        auto animation = dynamic_cast<QPropertyAnimation*>(group->animationAt(i));
        auto skier = dynamic_cast<SkierItem*>(animation->targetObject());
        if (skier) {
            skier->show();
            skier->sem.release();

        }
    }
    delete group;
}

void SlopeDisplay::finishedCCarAnimation()
{
    auto group=dynamic_cast<QAnimationGroup*>(sender());
    for(int i=0;i<group->animationCount();i++) {
        auto animation=dynamic_cast<QPropertyAnimation*>(group->animationAt(i));
        auto cableCar=dynamic_cast<CableCarItem*>(animation->targetObject());
        if (cableCar) {
            cableCar->sem.release();
        }
    }
    delete group;
}


