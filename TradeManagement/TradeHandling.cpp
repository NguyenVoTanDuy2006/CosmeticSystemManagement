#include "TradeManager.h"

void TradeManager::addTrade(const QString& productID, const LotInfo& shipment, int capital) {
    trades.push_back(std::make_shared<TradeIn>(productID, shipment.id, shipment.quantity, capital));
}

void TradeManager::addTrade(const QString& productID, int quantity, int revenue, const Client& client) {
    trades.push_back(std::make_shared<TradeOut>(productID, client, quantity, revenue));
}

bool TradeManager::deleteTrade(const QString& ID) {
    auto it = std::remove_if(trades.begin(), trades.end(),
        [&](const std::shared_ptr<Trade>& trade){ return trade->ID == ID; });

    if (it != trades.end()) {
        trades.erase(it, trades.end());
        return true;
    }
    return false;
}

std::shared_ptr<Trade> TradeManager::findTrade(const QString& ID) {
    for (const auto& trade: trades) {
        if (trade->ID == ID){ return trade; }
    }
    return nullptr;
}