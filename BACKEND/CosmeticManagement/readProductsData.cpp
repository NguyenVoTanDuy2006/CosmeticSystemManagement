#include "productManager.h"

bool productManager::readData() {
    QFile productsListFile(getPath("ListProducts"));
    if (!productsListFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open ListProducts for reading.";
        return false;
    }
    QTextStream in(&productsListFile);
    products.clear();
    products.emplace_back(); // Add dummy product back

    QString ID;
    while (in.readLineInto(&ID) && !ID.trimmed().isEmpty()) {
        Product temp(ID);
        QFile productDataFile(getPath(ID));
        if (!productDataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Could not open product data file:" << getPath(ID);
            continue;
        }
        QTextStream productDataStream(&productDataFile);
        productDataStream >> temp;
        products.push_back(temp);
        productDataFile.close();
    }
    productsListFile.close();

    if (products.size() > 1) { // More than just the dummy
        Product::setLastIDNumber(products.back().reverseID() + 1);
    }
    return true;
}