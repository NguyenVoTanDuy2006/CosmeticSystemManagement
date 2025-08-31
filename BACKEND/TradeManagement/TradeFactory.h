#ifndef TRADEFACTORY_H
#define TRADEFACTORY_H

#include "libs.hpp"
#include "TradeIn.h"
#include "TradeOut.h"

class TradeFactory
{
public:
    // ID có dạng yyyyake_shared<TradMMddhhmmss_SPxxxxx_T, T là type
    static std::shared_ptr<Trade> getTrade(const QString &ID)
    {
        if (ID.endsWith("_0"))
        {
            return std::make_shared<TradeIn>(ID);
        }
        else if (ID.endsWith("_1"))
        {
            return std::make_shared<TradeOut>(ID);
        }
        return nullptr;
    }
};

#endif // TRADEFACTORY_H