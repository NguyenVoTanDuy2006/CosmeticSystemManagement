#include "TradeManager.h"

QString TradeManager::getPath(const QString& name) {
    QDir dataPath("Data/TradeData");
    if (!dataPath.exists())
    {
        dataPath.mkpath("Data/TradeData");
    }
    return "Data/TradeData/" + name + ".txt";
}

TradeManager::~TradeManager() {
    trades.clear();
}