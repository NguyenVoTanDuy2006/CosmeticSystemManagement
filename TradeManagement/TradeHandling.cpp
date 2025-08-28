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
        const auto begin_of_remove = it;
        for (; it != trades.end(); ++it)
        {
            QFile::remove(getPath((*it)->getID()));
        }
        trades.erase(begin_of_remove, trades.end());
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

std::vector<std::shared_ptr<Trade>> TradeManager::getFilteredTrades(
    const std::vector<std::shared_ptr<baseTradeFilterCondition>>& conditions) const
{
    std::vector<std::shared_ptr<Trade>> filteredTrades;
    for (const auto& trade: trades)
    {
        if (std::all_of(conditions.begin(), conditions.end(),
                [&](const auto& condition){return condition->check(trade);}))
        {
            filteredTrades.push_back(trade);
        }
    }
    return filteredTrades;
}