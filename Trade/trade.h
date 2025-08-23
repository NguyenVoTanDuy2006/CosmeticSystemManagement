#pragma once
#include "libs.hpp"

enum class IO_TYPE: int
{
    IN = 0,
    OUT = 1,
};

class Trade
{
protected:
    QString ID;
    QString productID;
    QDateTime tradeTime;
    int quantity;

    QString createID(int type) const;
    static QDateTime getCurrentTime();

public:
    virtual ~Trade() = default;
    Trade();
    explicit Trade(const QString& ID);
    Trade(const QString& productID, int quantity);
    Trade(const QDateTime& time, const QString& productID, int quantity);

    QString getID() const { return ID; }

    bool operator<(const Trade& other) const;
    bool operator<=(const Trade& other) const;

    virtual void readData(QTextStream& is) = 0;
    virtual void writeData(QTextStream& os) = 0;

    friend class TradeManager;
};