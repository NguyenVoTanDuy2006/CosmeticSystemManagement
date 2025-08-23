#include "product.hpp"

QTextStream& operator>>(QTextStream& is, Product& ojb) {
    is >> ojb.info;
    if (ojb.info.name == "null") return is;

    ojb.Shipments.clear();
    while (!is.atEnd()) {
        QString line = is.readLine();
        if (line.trimmed().isEmpty()) break;
        QTextStream lineStream(&line, QIODevice::ReadOnly);
        LotInfo temp;
        lineStream >> temp;
        ojb.Shipments.push_back(temp);
    }

    if (ojb.ID.isEmpty()) ojb.ID = Product::CreateID();
    return is;
}

QTextStream& operator<<(QTextStream& os, const Product& ojb) {
    if (ojb.ID.isEmpty()) {
        os << "null";
        return os;
    }
    os << ojb.info;
    for (const auto& shipment : ojb.Shipments)
        os << shipment << '\n';
    return os;
}