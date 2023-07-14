/**
* @brief implementation des méthodes de la classe Mine
* @file mine.cpp
* @author Hugo Ducommun
* @author Alexis Martins
*/
#include "mine.h"
#include "costs.h"
#include <pcosynchro/pcothread.h>

WindowInterface* Mine::interface = nullptr;

Mine::Mine(int unique_id, int fund, ItemType resource_mined)
    : Seller(fund, unique_id),/*, unique_id(unique_id),*/ resource_mined(resource_mined), nb_mined(0)
{
    assert(resource_mined == ItemType::Copper ||
           resource_mined == ItemType::Sand ||
           resource_mined == ItemType::Zinc);
    interface->consoleAppendText(unique_id, QString("Mine Created"));
    interface->updateFund(unique_id, fund);
}

std::map<ItemType, int> Mine::get_items_for_sale() {
    return stocks;
}

int Mine::buy(ItemType it, int qty) {

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

void Mine::run() {
    interface->consoleAppendText(unique_id, "[START] Mine routine");

    // Arrêt du programme pour la mine si l'arrêt des threads est demandé
    while (!PcoThread::thisThread()->stopRequested()) {
        /* TODO concurrence */

        int miner_cost = get_employee_salary(get_employee_that_produces(resource_mined));

        mutex.lock();
        if (money < miner_cost) {
            /* Pas assez d'argent */
            /* Attend des jours meilleurs */

            mutex.unlock();
            PcoThread::usleep(1000U);
            continue;
        }

        /* On peut payer un mineur */
        money -= miner_cost;
        mutex.unlock();

        /* Temps aléatoire borné qui simule le mineur qui mine */
        PcoThread::usleep((rand() % 100 + 1) * 10000);

        mutex.lock();

        /* Statistiques */
        nb_mined++;
        /* Incrément des stocks */
        stocks[resource_mined] += 1;

        mutex.unlock();

        /* Message dans l'interface graphique */
        interface->consoleAppendText(unique_id, QString("1 ") % get_item_name(resource_mined) %
                                     " has been mined");
        /* Update de l'interface graphique */
        interface->updateFund(unique_id, money);
        interface->updateStock(unique_id, &stocks);
    }
    interface->consoleAppendText(unique_id, "[STOP] Mine routine");
}

int Mine::get_material_cost() {
    return get_cost_per_unit(resource_mined);
}

ItemType Mine::get_resource_mined() {
    return resource_mined;
}

int Mine::get_amount_paid_to_miners() {
    return nb_mined * get_employee_salary(get_employee_that_produces(resource_mined));
}

void Mine::set_interface(WindowInterface *window_interface) {
    interface = window_interface;
}

SandMine::SandMine(int unique_id, int fund): Mine::Mine(unique_id, fund, ItemType::Sand) {}

CopperMine::CopperMine(int unique_id, int fund): Mine::Mine(unique_id, fund, ItemType::Copper) {}

ZincMine::ZincMine(int unique_id, int fund): Mine::Mine(unique_id, fund, ItemType::Zinc) {}
