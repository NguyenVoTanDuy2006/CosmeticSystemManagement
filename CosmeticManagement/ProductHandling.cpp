#include "productManager.h"

void productManager::addProduct(const productInfo &info) {
    products.push_back(Product(info));
}

void productManager::removeProduct(std::vector<QString> IDs) {
    auto it = std::remove_if(products.begin(), products.end(),
        [&](const Product& p)
        {
            return std::find(IDs.begin(), IDs.end(), p.getID()) != IDs.end();
        });

    const auto begin_of_remove = it;
    // remove file;
    for (; it != products.end(); ++it)
    {
        QFile::remove(std::_Simple_types<Product>::value_type::getPicturePath(it->getID()));
        QFile::remove(getPath(it->getID()));
    }
    products.erase(begin_of_remove, products.end());
}

Product& productManager::findProduct(const QString& name) {
    for (auto& product : products) {
        if (product.getInfo().name == name) return product;
    }
    return products[0]; // dummy product
}