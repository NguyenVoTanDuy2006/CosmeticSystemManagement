//
// Created by nguye on 8/24/2025.
//

#ifndef TRADEFILTERCONDITIONS_H
#define TRADEFILTERCONDITIONS_H

#include "TradeFactory.h"
#include <functional>

class baseTradeFilterCondition
{
    std::function<bool (std::shared_ptr<Trade>)> condition;

public:
    virtual ~baseTradeFilterCondition() = default;

    baseTradeFilterCondition(const std::function<bool (std::shared_ptr<Trade>)>& condition)
    {
        this->condition = condition;
    }

    bool check(const std::shared_ptr<Trade>& trade) const { return condition(trade); }
};

class InTradeCondition : public baseTradeFilterCondition
{
public:
    ~InTradeCondition() override = default;

    InTradeCondition()
        : baseTradeFilterCondition([](const std::shared_ptr<Trade>& trade)
        {
            return std::dynamic_pointer_cast<std::shared_ptr<TradeIn>>(trade) != nullptr;
        })
    {
    }
};

class OutTradeCondition : public baseTradeFilterCondition
{
public:
    ~OutTradeCondition() override = default;

    OutTradeCondition()
        : baseTradeFilterCondition([](const std::shared_ptr<Trade>& trade)
        {
            return std::dynamic_pointer_cast<std::shared_ptr<TradeOut>>(trade) != nullptr;
        })
    {
    }
};

class DateTradeCondition : public baseTradeFilterCondition
{
    QDateTime from, to;

public:
    ~DateTradeCondition() override = default;

    DateTradeCondition(QDateTime from, QDateTime to)
        : baseTradeFilterCondition([from, to](const std::shared_ptr<Trade>& trade)
          {
              return (from < trade->getTradeTime() && trade->getTradeTime() < to);
          }),
          from(from),
          to(to)

    {
    }
};


#endif //TRADEFILTERCONDITIONS_H
