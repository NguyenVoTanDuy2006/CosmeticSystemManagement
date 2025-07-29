//
// Created by nguye on 7/28/2025.
//

#include "TradeOut.h"


TradeOut::TradeOut(const string& ID): Trade(ID), revenue(-1){}

TradeOut::TradeOut(const string& productID, const Client& client, const int quantity, const int revenue):
    Trade(productID, quantity), client(client), revenue(revenue)
{
    this->ID = createID(static_cast<int>(IO_TYPE::OUT));
}

TradeOut::TradeOut(const string& productID, const Client& client, const int quantity, const tm& time,
                   const int revenue): Trade(time, productID, quantity), client(client), revenue(revenue)
{
    this->ID = createID(static_cast<int>(IO_TYPE::OUT));
}

void TradeOut::readData(istream& is)
{
    Trade::readData(is);
    is >> revenue >> ws;
    is >> client;
}

void TradeOut::writeData(ostream& os)
{
    Trade::writeData(os);
    os << client << endl;
    os << revenue;
}
