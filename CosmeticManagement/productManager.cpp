#include "productManager.h"

QString productManager::getPath(const QString& name) {
    QDir dataPath("Data/ProductData");
    if (!dataPath.exists())
        dataPath.mkpath(".");
    return "Data/ProductData/" + name + ".txt";
}

productManager::productManager() {
    products.emplace_back(); // dummy product
}

productManager::~productManager() {
    products.clear();
}