//
// Created by nguye on 8/28/2025.
//
#include "productManager.h"

bool productManager::importProduct(QString ID, const LotInfo& shipment)
{
    Product& product = findProductByID(ID);
    if (product.getID().compare(ID) == 0)
    {
        product.addShipment(shipment);
        return true;
    }
    return false; // cant find product;
}