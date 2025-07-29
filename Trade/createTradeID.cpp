#include  "trade.h"

string Trade::createID(const int type) const
{
    stringstream ID;
    ID << setw(2) << setfill('0')
        << tradeTime.tm_year << '/' << tradeTime.tm_mon << '/' << tradeTime.tm_mday << '/'
        << tradeTime.tm_hour << '/' << tradeTime.tm_min << '/' << tradeTime.tm_sec;
    ID << productID << type;
    return ID.str();
}
