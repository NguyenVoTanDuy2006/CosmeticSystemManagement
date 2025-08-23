#ifndef TRADEOUT_H
#define TRADEOUT_H

#include "trade.h"
#include "client.h"

class TradeOut : public Trade
{
private:
    Client client;
    int revenue = 0;

public:
    TradeOut() = default;
    explicit TradeOut(const QString& ID);
    TradeOut(const QString& productID, const Client& client, int quantity, int revenue);
    TradeOut(const QString& productID, const Client& client, int quantity, const QDateTime& time, int revenue);
    
    void readData(QTextStream& is) override;
    void writeData(QTextStream& os) override;

    friend class TradeManager;
};

#endif //TRADEOUT_H