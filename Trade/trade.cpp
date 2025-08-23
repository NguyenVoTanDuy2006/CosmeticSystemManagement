#include "trade.h"

Trade::Trade() {
    ID = "";
    productID = "";
    // tradeTime mặc định là null
    quantity = -1;
}

Trade::Trade(const QString& ID): ID(ID) {}

Trade::Trade(const QString& productID, int quantity): productID(productID), quantity(quantity) {
    tradeTime = getCurrentTime();
}

Trade::Trade(const QDateTime& time, const QString& productID, const int quantity):
    productID(productID), tradeTime(time), quantity(quantity) {}

bool Trade::operator<(const Trade& other) const {
    return this->tradeTime < other.tradeTime;
}

bool Trade::operator<=(const Trade& other) const {
    return this->tradeTime <= other.tradeTime;
}