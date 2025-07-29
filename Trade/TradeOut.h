//
// Created by nguye on 7/28/2025.
//

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
    explicit TradeOut(const string& ID);
    TradeOut(const string& productID, const Client& client, int quantity, int revenue);
    TradeOut(const string& productID, const Client& client, int quantity, const tm& time, int revenue);
    // data handling
    void readData(istream& is) override;
    void writeData(ostream& os) override;

    friend class TradeManagement;
};


#endif //TRADEOUT_H
