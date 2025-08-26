#include "productManager.h"

void productManager::addProduct(const productInfo &info) {
    products.push_back(Product(info));
}

void productManager::removeProduct(const QString& productID) {
    auto it = std::remove_if(products.begin(), products.end(),
        [&](const Product& p) { return p.getInfo().name == productID; });

    if (it != products.end()) {
        products.erase(it, products.end());
    }
}

Product& productManager::findProduct(const QString& name) {
    for (auto& product : products) {
        if (product.getInfo().name == name) return product;
    }
    return products[0]; // dummy product
}