#ifndef SELLER_H
#define SELLER_H

#include <QString>
#include <QStringBuilder>
#include <map>
#include <vector>
#include "costs.h"

enum class ItemType { Sand, Copper, Zinc, Brass, Glass, Spectacles, Nothing };

extern int get_cost_per_unit(ItemType item);
QString get_item_name(ItemType item);

enum class EmployeeType { Miner, Smelter, Engineer, Craftsman };

extern EmployeeType get_employee_that_produces(ItemType item);
extern int get_employee_salary(EmployeeType employee);

class Seller {
public:
    /**
     * @brief Seller
     * @param money money money !
     */
    Seller(int money, int unique_id) : money(money), unique_id(unique_id) {}

    /**
     * @brief get_items_for_sale
     * @return The list of items for sale
     */
    virtual std::map<ItemType, int> get_items_for_sale() = 0;

    /**
     * @brief Fonction permettant d'acheter des ressources au vendeur
     * @param Le type de ressource à acheter
     * @param Nombre de ressources voulant être achetées
     * @return La facture : côut de la ressource * le nombre, 0 si indisponible
     */
    virtual int buy(ItemType what, int qty) = 0;

    /**
     * @brief choose_random_seller
     * @param sellers
     * @return Returns a random seller from the sellers vector
     */
    static Seller* choose_random_seller(std::vector<Seller*>& sellers);

    /**
     * @brief Chooses a random item type from an items for sale map
     * @param items_for_sale
     * @return Returns the item type
     */
    static ItemType choose_random_item(std::map<ItemType, int>& items_for_sale);

    int get_fund() { return money; }

    int get_unique_id() { return unique_id; }

protected:
    /**
     * @brief stocks : Type, Quantité
     */
    std::map<ItemType, int> stocks;
    int money;
    int unique_id;
};

#endif // SELLER_H
