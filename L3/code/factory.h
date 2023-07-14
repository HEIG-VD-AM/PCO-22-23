/**
* @brief Déclaration de la class Factory. Vous pouvez ajoutez des choses, mais ne rien changer.
* @file factory.h
* @author Hugo Ducommun
* @author Alexis Martins
*/

#ifndef FACTORY_H
#define FACTORY_H
#include <vector>
#include <pcosynchro/pcomutex.h>
#include "windowinterface.h"
#include "seller.h"

class Wholesale;

/**
 * @brief La classe permet l'implémentation d'une usine et de ces fonctions
 *        de ventes et d'achats.
 */
class Factory : public Seller
{
public:
    /**
     * @brief Constructeur de la classe Factory
     * @param Fonds initiale
     * @param La ressources qui sera construite par l'usine
     * @param Vecteurs de ressources nécessaires pour construire l'objet
     */
    Factory(int unique_id, int fund, ItemType built_item, std::vector<ItemType> ressources_needed);

    /**
     * @brief Est la routine d'appel aux fonctions privée pour que l'usine fonctionne continuellement.
     * fonction threadée
     */
    void run();

    std::map<ItemType, int> get_items_for_sale() override;
    int buy(ItemType it, int number) override;

    /**
     * @brief Permet d'accèder au coût du matériel produit par l'usine
     * @return Le côût du metérial produit
     */
    int get_material_cost();

    /**
     * @brief Retourne l'identifiant de l'objet produit par l'usine
     * @return Identifiant de l'usine.
     */
    ItemType get_item_built();

    /**
     * @brief Cette fonction permet d'affecter à une usine pluseurs grossistes pour pouvoir échanger avec eux.
     * @param Vecteur de wholesaler
     */
    void set_wholesalers(std::vector<Wholesale*> wholesalers);

    int get_amount_paid_to_workers();

    static void set_interface(WindowInterface* interface);

private:
    // Liste de grossiste auxquels l'usine peut acheter des ressources
    std::vector<Wholesale*> wholesalers;

    std::vector<ItemType> sort_ressources_by_stock();
    // Liste de ressources voulus pour la production d'un objet
    const std::vector<ItemType> ressources_needed;
    // Identifiant de l'objet produit par l'usine, selon l'enum Build_id
    const ItemType item_built;
    // Compte le nombre d'employé payé
    int nb_build;

    static WindowInterface* interface;

    PcoMutex mutex;


    /**
     * @brief Fonction privée permettant de vérifier si l'usine à toute les ressources
     *        nécessaire à la construction d'un objet.
     * @return true si elle a assez de ressources, false sinon.
     */
    bool verify_ressources();

    /**
     * @brief Achat de ressources chez les grossistes (wholesalers)
     */
    void order_ressources();

    /**
     * @brief Construction d'un objet par l'usine.
     */
    void build_item();
};


// Si dessous sont les trois types d'usines. Elle vont créer des usines en leur
// spécifiant les besoins en matérial et leur identifiant.

class BrassSmeltery : public Factory{
private:
public:
    BrassSmeltery(int unique_id, int fund);
};

class Glassmakers : public Factory {
private:
public:
    Glassmakers(int unique_id, int fund);
};

class SpectacleFactory : public Factory {
private:
public:
    SpectacleFactory(int unique_id, int fund);
};

#endif // FACTORY_H
