#include "seller.h"
#include <algorithm>
#include <random>
#include <cassert>

Seller *Seller::choose_random_seller(std::vector<Seller *> &sellers) {
    assert(sellers.size());
    std::vector<Seller*> out;
    std::sample(sellers.begin(), sellers.end(), std::back_inserter(out),
            1, std::mt19937{std::random_device{}()});
    return out.front();
}

ItemType Seller::choose_random_item(std::map<ItemType, int> &items_for_sale) {
    if (!items_for_sale.size()) {
        return ItemType::Nothing;
    }
    std::vector<std::pair<ItemType, int> > out;
    std::sample(items_for_sale.begin(), items_for_sale.end(), std::back_inserter(out),
            1, std::mt19937{std::random_device{}()});
    return out.front().first;
}

int get_cost_per_unit(ItemType item) {
    switch (item) {
        case ItemType::Sand : return SAND_COST;
        case ItemType::Copper : return COPPER_COST;
        case ItemType::Zinc : return ZINC_COST;
        case ItemType::Brass : return BRASS_COST;
        case ItemType::Glass : return GLASS_COST;
        case ItemType::Spectacles : return SPECTACLE_COST;
        default : return 0;
    }
}

QString get_item_name(ItemType item) {
    switch (item) {
        case ItemType::Sand : return "Sand";
        case ItemType::Copper : return "Copper";
        case ItemType::Zinc : return "Zinq";
        case ItemType::Brass : return "Brass";
        case ItemType::Glass : return "Glass";
        case ItemType::Spectacles : return "Spectacles";
        case ItemType::Nothing : return "Nothing";
        default : return "???";
    }
}

EmployeeType get_employee_that_produces(ItemType item) {
    switch (item) {
        case ItemType::Sand : /* fallthrough */
        case ItemType::Copper : /* fallthrough */
        case ItemType::Zinc : return EmployeeType::Miner;
        case ItemType::Brass : /* fallthrough */
        case ItemType::Glass : return EmployeeType::Engineer;
        case ItemType::Spectacles : return EmployeeType::Craftsman;
        default : return EmployeeType::Smelter; // Why not
    }
}

int get_employee_salary(EmployeeType employee) {
    switch (employee) {
        case EmployeeType::Miner : return MINER_COST;
        case EmployeeType::Smelter : return SMELTER_COST;
        case EmployeeType::Engineer : return ENGINEER_COST;
        case EmployeeType::Craftsman : return CRAFTSMAN_COST;
        default : return 0;
    }
}
