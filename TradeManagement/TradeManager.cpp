#include "TradeManager.h"

QString TradeManager::getPath(const QString& name) {
    QString basePath = QDir::currentPath() + "/Data/TradeData/";
    QDir dataPath(basePath);
    if (!dataPath.exists())
    {
        dataPath.mkpath(".");
    }
    return basePath + name + ".txt";
}

std::shared_ptr<TradeManager> TradeManager::getInstance()
{
    static std::shared_ptr<TradeManager> instance = std::make_shared<TradeManager>();
    return instance;
}

TradeManager::~TradeManager() {
    trades.clear();
}
