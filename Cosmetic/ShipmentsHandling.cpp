#include "product.hpp"

void Product::addShipment(const LotInfo& lot)
{
    LotInfo& shipment = findShipment(lot.id);
    if (shipment.id != LotInfo::nullValue().id)
    {
        shipment.quantity += LotInfo::nullValue().quantity;
    }
    else
    {
        Shipments.push_back(lot);
        std::sort(Shipments.begin(), Shipments.end()
    }
}

void Product::removeShipment(LotInfo& lot)
{
    auto it = std::find_if(Shipments.begin(), Shipments.end(),
                           [&](const LotInfo& item) { return item.id == lot.id; });

    if (it != Shipments.end())
    {
        Shipments.erase(it);
    }
}

LotInfo& Product::findShipment(const QString& ID)
{
    for (auto& Shipment : Shipments)
    {
        if (Shipment.id == ID)
            return Shipment;
    }
    return LotInfo::nullValue();
}
