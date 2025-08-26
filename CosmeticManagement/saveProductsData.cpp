#include "productManager.h"

bool productManager::saveData() {
    removeProduct(""); // Keep logic if necessary

    QFile productListFile(getPath("ListProducts"));
    if (!productListFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open ListProducts for writing.";
        return false;
    }
    QTextStream productListStream(&productListFile);

    for (const auto& product : products) {
        if (product.getInfo().name.isEmpty()) continue; // Skip dummy product
        productListStream << product.ID << '\n'; // Assuming product ID is name

        QFile productDataFile(getPath(product.getInfo().name));
        if (productDataFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream productDataStream(&productDataFile);
            productDataStream << product;
            productDataFile.close();
        } else {
            qWarning() << "Could not open" << getPath(product.getInfo().name) << "for writing.";
        }
    }
    productListFile.close();
    return true;
}