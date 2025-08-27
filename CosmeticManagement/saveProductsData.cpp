#include "productManager.h"

bool productManager::saveData()
{
    QFile productListFile(getPath("ListProducts"));
    if (!productListFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Could not open ListProducts for writing.";
        return false;
    }
    QTextStream productListStream(&productListFile);

    for (const auto& product : products)
    {
        QString pID = product.getID();
        if (pID.isEmpty()) continue; // Skip dummy product
        productListStream << product.ID << '\n';

        QFile productDataFile(getPath(pID));
        if (productDataFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream productDataStream(&productDataFile);
            productDataStream << product;
            productDataFile.close();
        }
        else
        {
            qWarning() << "Could not open" << getPath(pID) << "for writing.";
        }
    }
    productListFile.close();
    return true;
}
