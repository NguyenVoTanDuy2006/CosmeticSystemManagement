#include "product.hpp"

QString Product::CreateID(int number) {
    return QString("SP%1").arg(number, 5, 10, QChar('0'));
}

QString Product::CreateID() {
    QString newID = QString("SP%1").arg(nextID, 5, 10, QChar('0'));
    nextID++;
    return newID;
}