/**
* @brief implementation des méthodes de la classe Factory
* @file factory.cpp
* @author Hugo Ducommun
* @author Alexis Martins
*/
#include "factory.h"
#include "mine.h"
#include "costs.h"
#include "wholesale.h"
#include <pcosynchro/pcothread.h>
#include <iostream>

WindowInterface* Factory::interface = nullptr;


Factory::Factory(int unique_id, int fund, ItemType item_built, std::vector<ItemType> ressources_needed)
    : Seller(fund, unique_id), /*unique_id(unique_id)*/ressources_needed(ressources_needed), item_built(item_built), nb_build(0)
{
    assert(item_built == ItemType::Brass ||
           item_built == ItemType::Glass ||
           item_built == ItemType::Spectacles);

    interface->updateFund(unique_id, fund);
    interface->consoleAppendText(unique_id, "Factory created");
}

void Factory::set_wholesalers(std::vector<Wholesale *> wholesalers) {
    Factory::wholesalers = wholesalers;

    for(Seller* seller: wholesalers){
        interface->set_link(unique_id, seller->get_unique_id());
    }
}

ItemType Factory::get_item_built() {
    return item_built;
}

int Factory::get_material_cost() {
    return get_cost_per_unit(item_built);
}

bool Factory::verify_ressources() {
    for (auto item : ressources_needed) {
        if (stocks[item] == 0) {
            return false;
        }
    }

    return true;
}

void Factory::build_item() {

    //TODO

    int builder_cost = get_employee_salary(get_employee_that_produces(item_built));

    mutex.lock();

    // Si on ne possède pas assez d'argent, on termine la fonction
    if (money < builder_cost) {

        mutex.unlock();
        return;
    }

    // Utilisation des ressources nécessaires
    for(ItemType item : ressources_needed) {
        stocks[item]--;
    }

    // On paye le constructeur
    money -= builder_cost;
    mutex.unlock();

    // Temps simulant l'assemblage d'un objet.
    PcoThread::usleep((rand() % 100) * 100000);

    mutex.lock();

    //TODO

    // Incrément pour le calcul final du coût total des builder
    nb_build++;

    // Mise à jour des stocks après construction
    stocks[item_built]++;
    mutex.unlock();

    interface->consoleAppendText(unique_id, "Factory have build a new object");
}

std::vector<ItemType> Factory::sort_ressources_by_stock() {

    mutex.lock();

    std::vector<ItemType> ret = ressources_needed;

    // Tri en fonction des ressources ayant le moins de stock
    std::sort(ret.begin(), ret.end(),
    [this](const ItemType& i1, const ItemType& i2) ->
    bool{ return stocks[i1] < stocks[i2]; });

    mutex.unlock();

    return ret;
}

void Factory::order_ressources() {

    //TODO - Itérer sur les ressources_needed et les wholesalers disponible
    for (auto it : sort_ressources_by_stock()) {
        for (Wholesale* ws : wholesalers) {

            mutex.lock();

            int price = get_cost_per_unit(it);

            /* On vérifie que l'on ait l'argent et que le vendeur ait pu nous vendre
            son objet, la vérification pour le stock se fait du côté du vendeur */
            if (money >= price && ws->buy(it, 1) == price) {

                // Achat de ressources
                money -= price;
                stocks[it]++;
            }

            mutex.unlock();
        }
    }
    //Temps de pause pour éviter trop de demande
    PcoThread::usleep(10 * 100000);
}

void Factory::run() {
    if (wholesalers.empty()) {
        std::cerr << "You have to give to factories wholesalers to sales their ressources" << std::endl;
        return;
    }
    interface->consoleAppendText(unique_id, "[START] Factory routine");

    // Arrêt du programme pour l'usine si l'arrêt des threads est demandé
    while (!PcoThread::thisThread()->stopRequested()) {
        if (verify_ressources()) {
            build_item();
        } else {
            order_ressources();
        }
        interface->updateFund(unique_id, money);
        interface->updateStock(unique_id, &stocks);
    }
    interface->consoleAppendText(unique_id, "[STOP] Factory routine");
}

std::map<ItemType, int> Factory::get_items_for_sale() {
    return std::map<ItemType, int>({{item_built, stocks[item_built]}});
}

int Factory::buy(ItemType it, int qty) {

    mutex.lock();

    auto itemsForSale = get_items_for_sale();

    // On vérifie que l'objet est présent dans la liste et en quantité suffisante
    if(itemsForSale.find(it) != itemsForSale.end() && qty > 0
       && qty <= itemsForSale[it]) {

        int cost = get_cost_per_unit(it) * qty;

        // Vente des ressources
        money += cost;
        stocks[it] -= qty;

        mutex.unlock();
        return cost;
    }

    mutex.unlock();

    return 0;
}

int Factory::get_amount_paid_to_workers() {
    return Factory::nb_build * get_employee_salary(get_employee_that_produces(item_built));
}

void Factory::set_interface(WindowInterface *window_interface) {
    interface = window_interface;
}

BrassSmeltery::BrassSmeltery(int unique_id, int fund) :
    Factory::Factory(unique_id, fund, ItemType::Brass, {ItemType::Copper, ItemType::Zinc}) {}

Glassmakers::Glassmakers(int unique_id, int fund) :
    Factory::Factory(unique_id, fund, ItemType::Glass, {ItemType::Sand}) {}

SpectacleFactory::SpectacleFactory(int unique_id, int fund) :
    Factory::Factory(unique_id, fund, ItemType::Spectacles, {ItemType::Brass, ItemType::Glass}) {}
