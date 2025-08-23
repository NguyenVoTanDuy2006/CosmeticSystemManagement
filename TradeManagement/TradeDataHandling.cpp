#include "TradeManager.h"

bool TradeManager::readData() {
    QFile tradeListFile(getPath("TradeList"));
    if (!tradeListFile.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    
    QTextStream in(&tradeListFile);
    trades.clear();

    QString ID;
    while (in.readLineInto(&ID) && !ID.trimmed().isEmpty()) {
        auto trade = TradeFactory::getTrade(ID);
        if (!trade) continue;

        QFile tradeDataFile(getPath(ID));
        if (!tradeDataFile.open(QIODevice::ReadOnly | QIODevice::Text)) continue;
        
        QTextStream tradeDataStream(&tradeDataFile);
        trade->readData(tradeDataStream);
        trades.push_back(trade);
        tradeDataFile.close();
    }
    tradeListFile.close();
    return true;
}

bool TradeManager::writeData() const {
    QFile tradeListFile(getPath("TradeList"));
    if (!tradeListFile.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&tradeListFile);
    for (const auto& trade: trades) {
        out << trade->ID << '\n';

        QFile tradeDataFile(getPath(trade->ID));
        if (tradeDataFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream tradeDataStream(&tradeDataFile);
            trade->writeData(tradeDataStream);
            tradeDataFile.close();
        }
    }
    tradeListFile.close();
    return true;
}