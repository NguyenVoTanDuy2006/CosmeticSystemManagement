#include "TradeOut.h"

TradeOut::TradeOut(const QString& ID): Trade(ID), revenue(-1){}

TradeOut::TradeOut(const QString& productID, const Client& client, const int quantity, const int revenue):
    Trade(productID, quantity), client(client), revenue(revenue) {
    this->ID = createID(static_cast<int>(IO_TYPE::OUT));
}

TradeOut::TradeOut(const QString& productID, const Client& client, const int quantity, const QDateTime& time,
                   const int revenue): Trade(time, productID, quantity), client(client), revenue(revenue) {
    this->ID = createID(static_cast<int>(IO_TYPE::OUT));
}

void TradeOut::readData(QTextStream& is) {
    Trade::readData(is); // Gọi hàm của lớp cha
    is >> revenue;
    is.skipWhiteSpace();
    is >> client;
}

void TradeOut::writeData(QTextStream& os) {
    Trade::writeData(os); // Gọi hàm của lớp cha
    os << revenue << '\n';
    os << client << '\n';
}