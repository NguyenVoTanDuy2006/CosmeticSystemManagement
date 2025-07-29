#pragma once

#include "time.hpp"

enum class IO_TYPE: int
{
    IN = 0,
    OUT = 1,
};

class Trade
{
protected:
    string ID;
    string productID;
    tm tradeTime;
    int quantity;

    string createID(int type) const;
    static tm getCurrentTime();

public:
    virtual ~Trade() = default;
    Trade();
    explicit Trade(const string& ID);
    Trade(const string& productID, int quantity);
    Trade(const tm& time, const string& productID, int quantity);

    bool operator <(const Trade& other) const;
    bool operator <=(const Trade& other) const;

    virtual void readData(istream& is) = 0;
    virtual void writeData(ostream& os) = 0;

    friend class TradeManagement;
};
