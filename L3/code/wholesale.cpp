/**
* @brief implementation des méthodes de la classe Wholesale
* @file wholesale.cpp
* @author Hugo Ducommun
* @author Alexis Martins
*/

#include "wholesale.h"
#include "factory.h"
#include "costs.h"
#include <iostream>
#include <pcosynchro/pcothread.h>

WindowInterface* Wholesale::interface = nullptr;

Wholesale::Wholesale(int unique_id, int fund)
    : Seller(fund, unique_id)
{
    interface->updateFund(unique_id, fund);
    interface->consoleAppendText(unique_id, "Wholesaler Created");
}

void Wholesale::set_sellers(std::vector<Seller*> sellers) {
    this->sellers = sellers;

    for(Seller* seller: sellers){
        interface->set_link(unique_id, seller->get_unique_id());
    }
}

void Wholesale::buy_ressources() {

    mutex.lock();
    auto s = Seller::choose_random_seller(sellers);
    auto m = s->get_items_for_sale();
    auto i = Seller::choose_random_item(m);
    mutex.unlock();

    if (i == ItemType::Nothing) {
        /* Nothing to sell... */
        return;
    }

    int qty = rand() % 5 + 1;
    int price = qty * get_cost_per_unit(i);

    interface->consoleAppendText(unique_id, QString("I would like to buy %1 of ").arg(qty) %
                                 get_item_name(i) % QString(" which would cost me %1").arg(price));

    /* TODO */
    mutex.lock();

    /* On vérifie que l'on ait l'argent et que le vendeur ait pu nous vendre
    son objet, la vérification pour le stock se fait du côté du vendeur */
    if(money >= price && s->buy(i, qty) == price) {

        // Achat des ressources
        money -= price;
        stocks[i] += qty;
    }

    mutex.unlock();
}

void Wholesale::run() {

    if (sellers.empty()) {
        std::cerr << "You have to give factories and mines to a wholeseler before launching is routine" << std::endl;
        return;
    }

    interface->consoleAppendText(unique_id, "[START] Wholesaler routine");

    // Arrêt du programme pour le grossiste si l'arrêt des threads est demandé
    while (!PcoThread::thisThread()->stopRequested()) {
        /* TODO synchronisation */
        buy_ressources();
        interface->updateFund(unique_id, money);
        interface->updateStock(unique_id, &stocks);
        //Temps de pause pour espacer les demandes de ressources
        PcoThread::usleep((rand() % 10 + 1) * 100000);
    }
    interface->consoleAppendText(unique_id, "[STOP] Wholesaler routine");


}

std::map<ItemType, int> Wholesale::get_items_for_sale() {
    return stocks;
}

int Wholesale::buy(ItemType it, int qty) {

    mutex.lock();

    auto itemsForSale = get_items_for_sale();

    // On vérifie que l'objet est présent dans la liste et en quantité suffisante
    if(itemsForSale.find(it) != itemsForSale.end() && qty > 0
       && qty <= itemsForSale[it]) {

        int cost = get_cost_per_unit(it) * qty;

        // Vente de l'objet
        money += cost;
        stocks[it] -= qty;

        mutex.unlock();

        return cost;
    }

    mutex.unlock();
    return 0;
}

void Wholesale::set_interface(WindowInterface *window_interface) {
    interface = window_interface;
}
