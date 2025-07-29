//
// Created by nguye on 7/29/2025.
//

#ifndef TRADEFACTORY_H
#define TRADEFACTORY_H

#include "libs.hpp"
#include "TradeIn.h"
#include "TradeOut.h"

class TradeFactory
{
public:
    static shared_ptr<Trade> getTrade(string ID)
    {
        int type = ID.back() - '0';
        switch (static_cast<IO_TYPE>(type))
        {
        case IO_TYPE::IN:
            return make_shared<TradeIn>(ID);
        case IO_TYPE::OUT:
            return make_shared<TradeOut>(ID);
        default:
            return nullptr;
        }
    }
};

#endif //TRADEFACTORY_H
