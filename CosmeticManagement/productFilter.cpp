#include "product manager.h"

std::vector<Product> productManager::filterByBrand(const QString& brand) const {
    std::vector<Product> ret;
    for (const auto& product : products) {
        if (product.getInfo().brand == brand) {
            ret.push_back(product);
        }
    }
    return ret;
}

std::vector<Product> productManager::filterByType(const TYPE type) const {
    std::vector<Product> ret;
    for (const auto& product : products) {
        if (product.getInfo().type == type) {
            ret.push_back(product);
        }
    }
    return ret;
}