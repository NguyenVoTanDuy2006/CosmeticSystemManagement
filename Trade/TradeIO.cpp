#include "trade.h"

void Trade::readData(istream& is)
{
    getline(is, productID);
    is >> this->tradeTime >> this->quantity;
    is >> ws;
}

void Trade::writeData(ostream& os)
{
    os  << this->productID << endl
        << this->tradeTime << endl
        << this->quantity << endl;
}
