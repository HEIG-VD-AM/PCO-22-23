#include "display.h"
#include <iostream>

constexpr double SCENEWIDTH = 1000.0;
constexpr double SCENELENGTH = 1000.0;
constexpr double ELEMENT_WIDTH = 80.0;


static Display* theDisplay;

MineItem::MineItem() = default;
FactoryItem::FactoryItem() = default;
WholesalerItem::WholesalerItem() = default;
RessourceItem::RessourceItem() = default;
ProductionItem::ProductionItem() = default;


void Display::place_ressources(int x, int y, int id){
    QPixmap fund(QString(":images/funds_color.png"));
    QPixmap _fund = fund.scaledToWidth(ELEMENT_WIDTH / 3);
    auto fund_item = new RessourceItem();
    fund_item->setPixmap(_fund);
    fund_item->setPos(x, y - (ELEMENT_WIDTH / 3));
    m_scene->addItem(fund_item);
    funds[id] = new QLabel(this);
    funds[id]->setText("Waiting...");
    funds[id]->setGeometry(QRect(x, y - 3 * (ELEMENT_WIDTH / 3), 70, 30));

    QPixmap brass(QString(":images/brass.png"));
    QPixmap _brass = brass.scaledToWidth(ELEMENT_WIDTH / 3);
    auto brass_item = new RessourceItem();
    brass_item->setPixmap(_brass);
    brass_item->setPos(x + (ELEMENT_WIDTH), y - (ELEMENT_WIDTH / 3));
    m_scene->addItem(brass_item);
    brasss[id] = new QLabel(this);
    brasss[id]->setText("Waiting...");
    brasss[id]->setGeometry(QRect(x + (ELEMENT_WIDTH), y -  3 * (ELEMENT_WIDTH / 3), 70, 30));

    QPixmap copper(QString(":images/copper.png"));
    QPixmap _copper = copper.scaledToWidth(ELEMENT_WIDTH / 3);
    auto copper_item = new RessourceItem();
    copper_item->setPixmap(_copper);
    copper_item->setPos(x + (ELEMENT_WIDTH), y);
    m_scene->addItem(copper_item);
    coppers[id] = new QLabel(this);
    coppers[id]->setText("Waiting...");
    coppers[id]->setGeometry(QRect(x + (ELEMENT_WIDTH), y - 2 * (ELEMENT_WIDTH / 3), 70, 30));

    QPixmap glace(QString(":images/glace.png"));
    QPixmap _glace = glace.scaledToWidth(ELEMENT_WIDTH / 3);
    auto glace_item = new RessourceItem();
    glace_item->setPixmap(_glace);
    glace_item->setPos(x + (ELEMENT_WIDTH), y + (ELEMENT_WIDTH / 3));
    m_scene->addItem(glace_item);
    glaces[id] = new QLabel(this);
    glaces[id]->setText("Waiting...");
    glaces[id]->setGeometry(QRect(x + (ELEMENT_WIDTH), y - 1 * (ELEMENT_WIDTH / 3), 70, 30));

    QPixmap sand(QString(":images/sand.png"));
    QPixmap _sand = sand.scaledToWidth(ELEMENT_WIDTH / 3);
    auto sand_item = new RessourceItem();
    sand_item->setPixmap(_sand);
    sand_item->setPos(x + (ELEMENT_WIDTH), y + 2 * (ELEMENT_WIDTH / 3));
    m_scene->addItem(sand_item);
    sands[id] = new QLabel(this);
    sands[id]->setText("Waiting...");
    sands[id]->setGeometry(QRect(x + (ELEMENT_WIDTH), y, 70, 30));

    QPixmap spectacle(QString(":images/spectacle.png"));
    QPixmap _spectacle = spectacle.scaledToWidth(ELEMENT_WIDTH / 3);
    auto spectacle_item = new RessourceItem();
    spectacle_item->setPixmap(_spectacle);
    spectacle_item->setPos(x + (ELEMENT_WIDTH), y + 3 * (ELEMENT_WIDTH / 3));
    m_scene->addItem(spectacle_item);
    spectacles[id] = new QLabel(this);
    spectacles[id]->setText("Waiting...");
    spectacles[id]->setGeometry(QRect(x + (ELEMENT_WIDTH), y + 1 * (ELEMENT_WIDTH / 3), 70, 30));

    QPixmap zinc(QString(":images/zinc.png"));
    QPixmap _zinc = zinc.scaledToWidth(ELEMENT_WIDTH / 3);
    auto zinc_item = new RessourceItem();
    zinc_item->setPixmap(_zinc);
    zinc_item->setPos(x + (ELEMENT_WIDTH), y + 4 * (ELEMENT_WIDTH / 3));
    m_scene->addItem(zinc_item);
    zincs[id] = new QLabel(this);
    zincs[id]->setText("Waiting...");
    zincs[id]->setGeometry(QRect(x + (ELEMENT_WIDTH), y + 2 * (ELEMENT_WIDTH / 3), 70, 30));
}

Display::Display(unsigned int nbMines, unsigned int nbFactories, unsigned int nbWholesalers, QWidget *parent) :
    QGraphicsView(parent)
{
    theDisplay = this;

    m_scene = new QGraphicsScene(this);

    funds = std::vector<QLabel*>(nbMines + nbFactories + nbWholesalers);
    brasss = std::vector<QLabel*>(nbMines + nbFactories + nbWholesalers);
    coppers = std::vector<QLabel*>(nbMines + nbFactories + nbWholesalers);
    sands = std::vector<QLabel*>(nbMines + nbFactories + nbWholesalers);
    glaces = std::vector<QLabel*>(nbMines + nbFactories + nbWholesalers);
    spectacles = std::vector<QLabel*>(nbMines + nbFactories + nbWholesalers);
    zincs = std::vector<QLabel*>(nbMines + nbFactories + nbWholesalers);
//    m_productItem = std::vector<ProductionItem*>(nbMines + nbFactories + nbWholesalers);

    //std::cout << nbMines + nbFactories + nbWholesalers << std::endl;

    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    this->setMinimumHeight(SCENEWIDTH);
    this->setMinimumWidth(SCENELENGTH);
    this->setScene(m_scene);

    for (unsigned int i = 0; i < nbMines; ++i){
        QPixmap img(QString(":images/mine_color.png"));
        QPixmap minePixmap;
        int x = 0 + SCENELENGTH / 6;//QRandomGenerator::system()->bounded(SCENELENGTH / 3);
        int y = (SCENEWIDTH / nbMines) * i + SCENEWIDTH / nbMines / 2;

        minePixmap = img.scaledToWidth(ELEMENT_WIDTH);
        auto mine = new ProductionItem();
        mine->setPixmap(minePixmap);
        mine->setPos(x, y);
        m_scene->addItem(mine);
        m_productItem.push_back(mine);

        place_ressources(x, y, i);

    }

    for (unsigned int i = 0; i < nbWholesalers; ++i) {
        QPixmap img(QString(":images/wholesaler_color.png"));
        QPixmap wholesalerPixmap;
        int x = (SCENELENGTH / 3) + (SCENELENGTH / 6);
//        int x = (SCENELENGTH / 3) * 2 + (SCENELENGTH / 6);//QRandomGenerator::system()->bounded(SCENELENGTH / 3);
        int y = (SCENEWIDTH / nbWholesalers) * i + SCENEWIDTH / nbWholesalers / 2;

        wholesalerPixmap = img.scaledToWidth(ELEMENT_WIDTH);
        auto wholesaler = new ProductionItem();
        wholesaler->setPixmap(wholesalerPixmap);

        wholesaler->setPos(x, y);
        m_scene->addItem(wholesaler);
        m_productItem.push_back(wholesaler);

        place_ressources(x, y, i + nbMines);
    }

    for (unsigned int i = 0; i < nbFactories; ++i) {
        QPixmap img(QString(":images/factory_color.png"));
        QPixmap factoryPixmap;
        int x = (SCENELENGTH / 3) * 2 + (SCENELENGTH / 6);
//        int x = (SCENELENGTH / 3) + (SCENELENGTH / 6);//QRandomGenerator::system()->bounded(SCENELENGTH / 3);
        int y = (SCENEWIDTH / nbFactories) * i + SCENEWIDTH / nbFactories / 2;

        factoryPixmap = img.scaledToWidth(ELEMENT_WIDTH);
        auto factory = new ProductionItem();
        factory->setPixmap(factoryPixmap);
        factory->setPos(x, y);
        m_scene->addItem(factory);
        m_productItem.push_back(factory);

        place_ressources(x, y, i + nbMines + nbWholesalers);

    }
}

void Display::update_fund(int idx, QString fund) {
    this->funds[idx]->setText(fund);
}

void Display::set_link(int from, int to){
    QLine line(m_productItem[from]->pos().toPoint(), m_productItem[to]->pos().toPoint());
    m_scene->addLine(line);
}

void Display::update_stocks(int idx, std::map<ItemType, int>* stocks) {
    this->sands[idx]->setText(QString::number((*stocks)[ItemType::Sand]));
    this->coppers[idx]->setText(QString::number((*stocks)[ItemType::Copper]));
    this->zincs[idx]->setText(QString::number((*stocks)[ItemType::Zinc]));
    this->glaces[idx]->setText(QString::number((*stocks)[ItemType::Glass]));
    this->brasss[idx]->setText(QString::number((*stocks)[ItemType::Brass]));
    this->spectacles[idx]->setText(QString::number((*stocks)[ItemType::Spectacles]));
}
