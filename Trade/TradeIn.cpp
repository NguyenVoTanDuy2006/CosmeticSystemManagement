//
// Created by nguye on 7/28/2025.
//

#include "TradeIn.h"

TradeIn::TradeIn(): Trade()
{
    LotID = "";
    capital  = -1;
}

TradeIn::TradeIn(const string& ID): Trade(ID), capital(-1){}

TradeIn::TradeIn(const string& productID, const string& LotID, const int quantity, const int capital): Trade(productID,  quantity), LotID(LotID), capital(capital)
{
    ID = createID(static_cast<int>(IO_TYPE::IN));
}

TradeIn::TradeIn(const string& productID, const string& LotID, const int quantity, const int capital, const tm& time): Trade(time, productID, quantity), LotID(LotID), capital(capital)
{
    ID = createID(static_cast<int>(IO_TYPE::IN));
}

void TradeIn::readData(istream& is)
{
    Trade::readData(is);
    getline(is, LotID);
    is >> capital >> ws;
}

void TradeIn::writeData(ostream& os)
{
    Trade::writeData(os);
    os << LotID << endl;
    os << capital<< endl;
}
