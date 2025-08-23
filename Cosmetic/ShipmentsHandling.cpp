#include "product.hpp"

void Product::addShipment(const LotInfo &lot) {
    Shipments.push_back(lot);
    std::sort(Shipments.begin(), Shipments.end());
}

void Product::removeShipment(LotInfo &lot) {
    auto it = std::find_if(Shipments.begin(), Shipments.end(),
        [&](const LotInfo& item) { return item.id == lot.id; });

    if (it != Shipments.end()) {
        Shipments.erase(it);
    }
}

LotInfo &Product::findShipment(const QDateTime &date) {
    for (auto &Shipment : Shipments) {
        if (Shipment.HSD.date() == date.date())
            return Shipment;
    }
    if (Shipments.empty()) {
        throw std::runtime_error("Cannot find shipment in an empty list.");
    }
    return Shipments.front();
}