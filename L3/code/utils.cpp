/**
* @brief Implémentation des méthodes de la class Utils, la seule que vous devez toucher est "endService()"
* @file utils.cpp
* @author Hugo Ducommun
* @author Alexis Martins
*/

#include "utils.h"


void Utils::endService() {
    //TODO
    std::cout << "It's time to end !" << std::endl;

    // On demande l'arrêt des threads
    for (auto& thread : threads) {
        thread->requestStop();
    }
}

std::vector<Mine*> create_mines(int nbMines, int id_start) {
    if (nbMines < 1){
        qInfo() << "Cannot make the programm work with less than 1 mines";
        exit(-1);
    }

    std::vector<Mine*> mines;

    for(int i = 0; i < nbMines; ++i) {
        switch(i % 3) {
            case 0:
                mines.push_back(new SandMine(i + id_start, MINES_FUND));
                break;

            case 1:
                mines.push_back(new ZincMine(i + id_start, MINES_FUND));
                break;

            case 2:
                mines.push_back(new CopperMine(i + id_start, MINES_FUND));
                break;
        }
    }


    return mines;
}

std::vector<Factory*> create_factories(int nbFactories, int id_start) {
    if (nbFactories < 1){
        qInfo() << "Cannot make the programm work with less than 1 Factory";
        exit(-1);
    }

    std::vector<Factory*> factories;

    for(int i = 0; i < nbFactories; ++i) {
        switch(i % 3) {
            case 0:
                factories.push_back(new BrassSmeltery(i + id_start, FACTORIES_FUND));
                break;

            case 1:
                factories.push_back(new Glassmakers(i + id_start, FACTORIES_FUND));
                break;

            case 2:
                factories.push_back(new SpectacleFactory(i + id_start, FACTORIES_FUND));
                break;
        }
    }


    return factories;
}

std::vector<Wholesale*> create_wholesaler(int nbWholesaler, int id_start) {
    if(nbWholesaler < 1){
        qInfo() << "Cannot launch the programm without any wholesaler";
        exit(-1);
    }

    std::vector<Wholesale*> wholesalers;

    for(int i = 0; i < nbWholesaler; ++i){
        wholesalers.push_back(new Wholesale(i + id_start, WHOLESALERS_FUND));
    }

    return wholesalers;
}


Utils::Utils(int nb_mines, int nb_factory, int nb_wholesale) {
    this->mines.resize(nb_mines);
    this->wholesalers.resize(nb_wholesale);
    this->factories.resize(nb_factory);

    this->mines = create_mines(nb_mines, 0);
    this->wholesalers = create_wholesaler(nb_wholesale, nb_mines);
    this->factories = create_factories(nb_factory, nb_mines + nb_wholesale);

    for(auto i = factories.begin(); i != factories.end(); ++i) {
        (*i)->set_wholesalers(wholesalers);
    }

    int minesByWholesaler = nb_mines / nb_wholesale;
    int minesShared = nb_mines % nb_wholesale;

    int factoriesByWholesaler = nb_factory / nb_wholesale;
    int factoriesShared = nb_factory % nb_wholesale;

    int count_mine = 0;
    int count_factory = 0;

    //Give random couple of mines and factory to Wholesalers
    for(auto& w : wholesalers) {

        std::vector<Mine*> tmp_mines(mines.begin() + count_mine, mines.begin() + count_mine + minesByWholesaler);
        tmp_mines.insert(tmp_mines.end(), mines.end() - minesShared, mines.end());

        std::vector<Factory*> tmp_factories(factories.begin() + count_factory, factories.begin() + count_factory + factoriesByWholesaler);
        tmp_factories.insert(tmp_factories.end(), factories.end() - factoriesShared, factories.end());

        count_mine += minesByWholesaler;
        count_factory += factoriesByWholesaler;

        std::vector<Seller*> sellers;
        for (auto& m : tmp_mines)
            sellers.push_back(static_cast<Seller*>(m));
        for (auto& f : tmp_factories)
            sellers.push_back(static_cast<Seller*>(f));
        w->set_sellers(sellers);
    }

    utilsThread = std::make_unique<PcoThread>(&Utils::run, this);
}

void Utils::run() {
    for(size_t i = 0; i < mines.size(); ++i) {
        threads.emplace_back(std::make_unique<PcoThread>(&Mine::run, mines[i]));
    }
    for(size_t i = 0; i < factories.size(); ++i) {
        threads.emplace_back(std::make_unique<PcoThread>(&Factory::run, factories[i]));
    }
    for(size_t i = 0; i < wholesalers.size(); ++i) {
        threads.emplace_back(std::make_unique<PcoThread>(&Wholesale::run, wholesalers[i]));
    }

    std::cout << "Appuyez sur [ENTER] pour terminer la simulation" << std::endl;

    QTextStream qin(stdin);
    QString Line = qin.readLine();

    this->endService();

    for (auto& thread : threads) {
        thread->join();
    }

    int start_fund = (MINES_FUND * int(mines.size()) + (FACTORIES_FUND * int(factories.size()) + (WHOLESALERS_FUND * int(wholesalers.size()))));
    int end_fund = 0;

    for(Mine* mine: mines) {
        end_fund += mine->get_fund();
        end_fund += mine->get_amount_paid_to_miners();
    }

    for(Factory* factory: factories) {
        end_fund += factory->get_fund();
        end_fund += factory->get_amount_paid_to_workers();
    }

    for(Wholesale* wholesale : wholesalers) {
        end_fund += wholesale->get_fund();
    }

    qInfo() << "The expected fund is : " << start_fund << " and you got at the end : " << end_fund;
}
