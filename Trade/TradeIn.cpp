#include "TradeIn.h"

TradeIn::TradeIn() {
    LotID = "";
    capital  = -1;
}

TradeIn::TradeIn(const QString& ID): Trade(ID), capital(-1){}

TradeIn::TradeIn(const QString& productID, const QString& LotID, const int quantity, const int capital): Trade(productID,  quantity), LotID(LotID), capital(capital) {
    ID = createID(static_cast<int>(IO_TYPE::IN));
}

TradeIn::TradeIn(const QString& productID, const QString& LotID, const int quantity, const int capital, const QDateTime& time): Trade(time, productID, quantity), LotID(LotID), capital(capital) {
    ID = createID(static_cast<int>(IO_TYPE::IN));
}

void TradeIn::readData(QTextStream& is) {
    Trade::readData(is); // Gọi hàm của lớp cha
    LotID = is.readLine();
    is >> capital;
    is.skipWhiteSpace();
}

void TradeIn::writeData(QTextStream& os) {
    Trade::writeData(os); // Gọi hàm của lớp cha
    os << LotID << '\n';
    os << capital << '\n';
}