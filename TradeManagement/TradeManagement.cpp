//
// Created by nguye on 7/29/2025.
//

#include "TradeManagement.h"

string TradeManagement::getPath(string name)
{
    return "Data/TradeData/" + name + ".txt";
}

TradeManagement::~TradeManagement()
{
    trades.clear();
}