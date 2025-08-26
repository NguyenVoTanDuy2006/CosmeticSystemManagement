#include "TradeManager.h"

QString TradeManager::getPath(const QString& name) {
    QString basePath = QDir::currentPath() + "Data/TradeData/";
    QDir dataPath(basePath);
    if (!dataPath.exists())
    {
        dataPath.mkpath(".");
    }
    return basePath + name + ".trade";
}

TradeManager::~TradeManager() {
    trades.clear();
}