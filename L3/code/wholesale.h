/**
* @brief Déclaration de la class Wholesale, vous pouvez ajoutez des choses, mais ne rien changer.
* @file wholesale.h
* @author Hugo Ducommun
* @author Alexis Martins
*/

#ifndef WHOLESALE_H
#define WHOLESALE_H
#include "seller.h"
#include <vector>
#include "windowinterface.h"

#include <pcosynchro/pcomutex.h>

class Mine;
class Factory;

/**
 * @brief La classe permet l'implémentation d'un grossiste et de ces fonctions
 *        de ventes et d'achats.
 */
class Wholesale : public Seller
{
private:
    // Vecteur de vendeurs (mines, usines) auxquels le grossiste peut acheter des ressources
    std::vector<Seller*> sellers;

    static WindowInterface* interface;

    PcoMutex mutex;

    /**
     * @brief Fonction permettant d'acheter des ressources à des usines ou des mines
     */
    void buy_ressources();
public:
    /**
     * @brief Constructeur de grossiste
     * @param Fond du grossiste à sa création
     */
    Wholesale(int unique_id, int fund);

    /**
     * @brief Routine d'exécution du grossiste
     */
    void run();

    std::map<ItemType, int> get_items_for_sale() override;
    int buy(ItemType it, int qty) override;

    /**
     * @brief Fonction permettant de lier des vendeurs
     * @param Vecteurs
     */
    void set_sellers(std::vector<Seller*> sellers);

    static void set_interface(WindowInterface* interface);
};

#endif // WHOLESALE_H
