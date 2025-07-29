//
// Created by nguye on 7/29/2025.
//

#include "TradeManagement.h"


bool TradeManagement::readData()
{
    auto tradeList = ifstream(getPath("TradeList"));
    if (tradeList.is_open() == false) return false;
    string ID;
    while (getline(tradeList, ID))
    {
        trades.push_back(TradeFactory::getTrade(ID));
        auto tradeData = ifstream(getPath("ID"));
        if (tradeData.is_open() == false) return false;
        trades.back()->readData(tradeData);
        tradeData.close();
    }
    tradeList.close();
    return true;
}

bool TradeManagement::writeData() const
{
    auto tradeList = ofstream(getPath("TradeList"));
    for (const auto& trade: trades)
    {
        tradeList << trade->ID << endl;
        auto tradeData = ofstream(getPath(trade->ID));
        trade->writeData(tradeData);
        tradeData.close();
    }
    tradeList.close();
    return true;
}
