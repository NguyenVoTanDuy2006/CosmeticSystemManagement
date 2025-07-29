#include "TradeManagement.h"

void TradeManagement::addTrade(const string& productID, const LotInfo& shipment, int capital)
{
    trades.push_back(make_shared<TradeIn>(productID, shipment.id, shipment.quantity, capital));
}

void TradeManagement::addTrade(const string& productID, int quantity, int revenue, const Client& client)
{
    trades.push_back(make_shared<TradeOut>(productID, client, quantity, revenue));
}

bool TradeManagement::deleteTrade(const string& ID)
{
    for (auto& trade: trades)
    {
        if (trade->ID == ID)
        {
            swap(trade, trades.back());
            trades.pop_back();
            sort(trades.begin(), trades.end());
            return true;
        }
    }
    return false;
}

shared_ptr<Trade> TradeManagement::findTrade(const string& ID)
{
    for (auto& trade: trades)
    {
        if (trade->ID == ID){return trade;}
    }
    return nullptr;
}
