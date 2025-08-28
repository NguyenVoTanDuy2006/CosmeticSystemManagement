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

std::shared_ptr<productManager> productManager::getInstance()
{
    static std::shared_ptr<productManager> instance = std::make_shared<productManager>();
    return instance;
}

productManager::~productManager() {
    products.clear();
}
