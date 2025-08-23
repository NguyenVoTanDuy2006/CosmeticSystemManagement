#ifndef TRADEIN_H
#define TRADEIN_H

#include "trade.h"

class TradeIn : public Trade
{
private:
    QString LotID;
    int capital = 0;
public:
    ~TradeIn() override = default;

    TradeIn();
    explicit TradeIn(const QString& ID);
    TradeIn(const QString& productID, const QString& LotID, int quantity, int capital);
    TradeIn(const QString& productID, const QString& LotID, int quantity, int capital, const QDateTime& time);

    void readData(QTextStream& is) override;
    void writeData(QTextStream& os) override;

    friend class TradeManager;
};

#endif //TRADEIN_H