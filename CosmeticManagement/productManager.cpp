#include "productManager.h"

QString productManager::getPath(const QString& name) {
    QString basePath = QDir::currentPath() + "Data/ProductData/";
    QDir dataPath(basePath);
    if (!dataPath.exists())
        dataPath.mkpath(".");
    return basePath + name + ".txt";
}

productManager::productManager() {
    products.emplace_back(); // dummy product
}

productManager::~productManager() {
    products.clear();
}