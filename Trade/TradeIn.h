//
// Created by nguye on 7/28/2025.
//

#ifndef TRADEIN_H
#define TRADEIN_H

#include "trade.h"

class TradeIn : public Trade
{
private:
    string LotID;
    int capital = 0;
public:
    ~TradeIn() override = default;

    TradeIn();
    explicit TradeIn(const string& ID);
    TradeIn(const string& productID, const string& LotID, int quantity, int capital);
    TradeIn(const string& productID, const string& LotID, int quantity, int capital, const tm& time);

    // data handling
    void readData(istream& is) override;
    void writeData(ostream& os) override;

    friend class TradeManagement;
};


#endif //TRADEIN_H
