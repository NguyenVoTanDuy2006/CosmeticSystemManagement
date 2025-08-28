//
// Created by nguye on 8/28/2025.
//

#include "productManager.h"
// lay san pham cu nhat toi moi nhat, xong lo nao la xoa lo do di
bool productManager::exportProduct(QString ID, int quantity)
{
    Product& product = findProduct(ID);
    if (product.ID.compare(ID) == 0 && product.getStock() >= quantity)
    {
        std::vector<LotInfo>& shipments = product.getShipments();
        auto it = shipments.begin();
        while ((quantity -= it->quantity) >= 0)
            ++it;

        quantity += it->quantity;
        shipments.erase(shipments.begin(), it);
        shipments.front().quantity -= quantity;
        return true;
    }
    return false; // cant find product or not enough stock
}
