#include "trade.h"

// Hàm này là của lớp cha và được override bởi các lớp con,
void Trade::readData(QTextStream& is) {
    productID = is.readLine();
    QString timeString = is.readLine();
    tradeTime = QDateTime::fromString(timeString, "yyyy/MM/dd hh:mm:ss");
    is >> quantity;
    is.skipWhiteSpace();
}

void Trade::writeData(QTextStream& os) {
    os << this->productID << '\n'
       << this->tradeTime.toString("yyyy/MM/dd hh:mm:ss") << '\n'
       << this->quantity << '\n';
}