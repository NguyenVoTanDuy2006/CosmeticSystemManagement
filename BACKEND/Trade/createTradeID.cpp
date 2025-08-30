#include "trade.h"

QString Trade::createID(const int type) const {
    // Tạo ID dựa trên thời gian, productID và loại giao dịch
    // Ví dụ: 20240730103055_SP00001_1 (OUT)
    return tradeTime.toString("yyyyMMddhhmmss") + "_" + productID + "_" + QString::number(type);
}