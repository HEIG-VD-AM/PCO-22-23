/**
* @brief Déclaration de la class Mine, vous pouvez ajoutez des choses, mais ne rien changer.
* @file mine.h
* @author Hugo Ducommun
* @author Alexis Martins
*/
#ifndef MINE_H
#define MINE_H
#include <QTimer>
#include <pcosynchro/pcomutex.h>
#include "windowinterface.h"
#include "costs.h"
#include "seller.h"

/**
 * @brief La classe offrant l'implémentation d'une mine et ces fonctions de ventes.
 */
class Mine : public Seller {
public:
    static void set_interface(WindowInterface* interface);

    /**
     * @brief Constructeur d'une mine
     * @param Fonds d'initialisation de la mine
     * @param Ressource minée
     */
    Mine(int unique_id, int fund, ItemType resource_mined);

    std::map<ItemType, int> get_items_for_sale() override;
    int buy(ItemType it, int qty) override;

    /**
     * @brief Routine de minage de ressources (fonction threadée)
     */
    void run();

    /**
     * @brief Fonction permettant de savoir quelle ressources la mine possède
     * @return Le type de minerai minés
     */
    ItemType get_resource_mined();

    /**
     * @brief Fonction permettant de savoir le coût de la ressources minée
     * @return Le coût de la ressource minée
     */
    int get_material_cost();

    int get_amount_paid_to_miners();

private:
    //int unique_id;
    // Identifiant du type de ressourcee miné
    const ItemType resource_mined;
    // Compte le nombre d'employé payé
    int nb_mined;

    static WindowInterface* interface;

    PcoMutex mutex;
};


// Si dessous sont les trois types de mines. Elle vont créer des mines en leur
// spécifiant leur identifiant.

class SandMine : public Mine {
private:
public:
    SandMine(int unique_id, int fund);
};


class CopperMine : public Mine {
private:
public:
    CopperMine(int unique_id, int fund);
};


class ZincMine : public Mine {
private:
public:
    ZincMine(int unique_id, int fund);
};




#endif // MINE_H
