#include "productManager.h"

QString productManager::getPath(const QString& name) {
    QString basePath = QDir::currentPath() + "/Data/ProductData/";
    QDir dataPath(basePath);
    if (!dataPath.exists())
        dataPath.mkpath(".");
    return basePath + name + ".txt";
}

productManager::productManager() {
    products.emplace_back(); // dummy product
}

productManager& productManager::getInstance()
{
    static productManager instance;
    return instance;
}

QString productManager::NameToID(const QString& name) const
{
    for (auto product : products)
    {
        if (product.getInfo().name == name) return product.getID();
    }
    return QString();
}

productManager::~productManager() {
    products.clear();
}
