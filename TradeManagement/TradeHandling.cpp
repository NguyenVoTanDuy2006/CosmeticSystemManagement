#include "TradeManager.h"

void TradeManager::addTrade(const QString& productID, const LotInfo& shipment, int capital) {
    trades.push_back(std::make_shared<TradeIn>(productID, shipment.id, shipment.quantity, capital));
}

void TradeManager::addTrade(const QString& productID, int quantity, int revenue, const Client& client) {
    trades.push_back(std::make_shared<TradeOut>(productID, client, quantity, revenue));
}

bool TradeManager::deleteTrade(std::vector<QString> IDs) {
    auto it = std::remove_if(trades.begin(), trades.end(),
        [&](const std::shared_ptr<Trade>& trade)
        {
            return find(IDs.begin(), IDs.end(), trade->ID) != IDs.end();
        });

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

std::vector<std::shared_ptr<Trade>> TradeManager::getFilterTrades(
    const std::vector<std::shared_ptr<baseTradeFilterCondition>>& conditions) const
{
    std::vector<std::shared_ptr<Trade>> filterTrades;
    bool flag;
    for (const auto& trade: trades)
    {
        flag = true;
        for (auto condition: conditions)
        {
            flag = (flag && condition->check(trade));
        }
        if (flag) { filterTrades.push_back(trade); }
    }
    return filterTrades;
}