#include "trade.h"

Trade::Trade()
{
    ID = "";
    productID = "";
    tradeTime = {};
    quantity = -1;
}

Trade::Trade(const string& ID): ID(ID)
{
}

Trade::Trade(const string& productID, int quantity): productID(productID), quantity(quantity)
{
    tradeTime = getCurrentTime();
}

Trade::Trade(const tm& time, const string& productID, const int quantity):
    productID(productID), tradeTime(time), quantity(quantity)
{
}

bool Trade::operator<(const Trade& other) const
{
    return this->tradeTime < other.tradeTime;
}

bool Trade::operator<=(const Trade& other) const
{
    return this->tradeTime <= other.tradeTime;
}
