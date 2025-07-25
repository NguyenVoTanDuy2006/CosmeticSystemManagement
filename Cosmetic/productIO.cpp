#include "product.hpp"

istream& operator>>(istream& is, Product& ojb)
{
    is >> ojb.info;
    LotInfo temp;
    while (is >> temp)
        ojb.Shipments.push_back(temp);
    if (ojb.ID == "") ojb.ID = ojb.CreateID();
    return is;
}

ostream& operator<<(ostream& os, const Product& ojb)
{
    if (ojb.ID == "")
        return (os << "null", os);
    os << ojb.info << '\n';
    for (auto shipment : ojb.Shipments)
        os << shipment << '\n';
    return os;
}
