#ifndef TRADEMANAGEMENT_H
#define TRADEMANAGEMENT_H

#include "shipment.hpp"
#include "TradeFactory.h"
#include "TradeFilterConditions.h"

class TradeManager {
private:
    std::vector<std::shared_ptr<Trade>> trades;
    static QString getPath(const QString& name);
public:
    TradeManager() = default;
    ~TradeManager();

    void addTrade(const QString& productID, const LotInfo& shipment, int capital);
    void addTrade(const QString& productID, int quantity, int revenue, const Client& client);
    bool deleteTrade(std::vector<QString> IDs);
    std::shared_ptr<Trade> findTrade(const QString& ID);

    int getTradeCount() const { return trades.size(); }

    //filter
    std::vector<std::shared_ptr<Trade>> getFilteredTrades(
        const std::vector<std::shared_ptr<baseTradeFilterCondition>>& conditions) const;

    bool readData();
    bool writeData() const;

};

#endif //TRADEMANAGEMENT_H