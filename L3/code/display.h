#ifndef DISPLAY_H
#define DISPLAY_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <pcosynchro/pcosemaphore.h>
#include <QLabel>
#include <QLine>

#include "seller.h"

class RessourceItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
        Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    public:
        RessourceItem();
        PcoSemaphore sem;
};

class MineItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
        Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    public:
        MineItem();
        PcoSemaphore sem;
};

class FactoryItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
        Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    public:
        FactoryItem();
        PcoSemaphore sem;
};

class WholesalerItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
        Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    public:
        WholesalerItem();
        PcoSemaphore sem;
};

class ProductionItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
        Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    public:
        ProductionItem();
        PcoSemaphore sem;
};

class Display : public QGraphicsView
{
    Q_OBJECT
public:
    Display(unsigned int nbMines, unsigned int nbFactories, unsigned int nbWholesalers, QWidget *parent);
    //~Display();

    std::vector<QLabel*> funds;
    std::vector<QLabel*> brasss;
    std::vector<QLabel*> coppers;
    std::vector<QLabel*> glaces;
    std::vector<QLabel*> sands;
    std::vector<QLabel*> spectacles;
    std::vector<QLabel*> zincs;

    std::vector<ProductionItem*> m_productItem;


    void update_stocks(int idx, std::map<ItemType, int>* stocks);
    void update_fund(int idx, QString fund);

    void set_link(int from, int to);

private:
    QGraphicsScene *m_scene;

    void place_ressources(int x, int y, int id);

public slots:

};

#endif // DISPLAY_H
