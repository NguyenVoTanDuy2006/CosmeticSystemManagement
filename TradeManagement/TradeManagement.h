//
// Created by nguye on 7/29/2025.
//

#ifndef TRADEMANAGEMENT_H
#define TRADEMANAGEMENT_H


#include "shipment.hpp"
#include "TradeFactory.h"


class TradeManagement {
private:
    vector<shared_ptr<Trade>> trades;
    static string getPath(string name);
public:
    TradeManagement() = default;
    ~TradeManagement();

    //trade handling
    void addTrade(const string& productID, const LotInfo& shipment, int capital);
    void addTrade(const string& productID, int quantity, int revenue, const Client& client);
    bool deleteTrade(const string& ID);
    shared_ptr<Trade> findTrade(const string& ID);

    //data handling
    bool readData();
    bool writeData() const;
    //filter

    // search

};



#endif //TRADEMANAGEMENT_H
