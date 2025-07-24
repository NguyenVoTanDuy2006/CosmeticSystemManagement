#include "product.hpp"

void product::addShipment(const LotInfo &lot)
{
    Shipments.push_back(lot);
    sort(Shipments.begin(), Shipments.end());
}

void product::removeShipment(LotInfo &lot)
{
    swap(lot, Shipments.back());
    Shipments.pop_back();
    sort(Shipments.begin(), Shipments.end());
}

LotInfo &product::findShipment(const tm &date)
{
    // int l = 0, r = Shipments.size() - 1, mid;
    // while(l <= r)
    // {
    //     mid = (l + r) / 2;
    //     if(Shipments[mid].HSD  == date)
    //         return Shipments[mid];
    //     if(Shipments[mid].HSD < date) l = mid + 1;
    //     else r = mid - 1;
    // }
    for (auto &Shipment : Shipments)
    {
        if (Shipment.HSD == date)
            return Shipment;
    }
    return Shipments[0];
}