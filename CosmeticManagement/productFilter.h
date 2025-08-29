//
// Created by nguye on 8/26/2025.
//

#ifndef PRODUCTFILTER_H
#define PRODUCTFILTER_H

#include "product.hpp"
#include <functional>


class baseProductFilter
{
    std::function<bool (Product)> condition;

public:
    virtual ~baseProductFilter() = default;

    baseProductFilter(const std::function<bool (Product)>& condition): condition(condition)
    {
    }

    bool check(const Product& product) const
    {
        return condition(product);
    }
};

class ProductBrandFilter : public baseProductFilter
{
public:
    ProductBrandFilter(QString Brand)
        :
        baseProductFilter([Brand](const Product& product)
        {
            return Brand == product.getInfo().brand;
        })
    {
    }
};

class ProductTypeFilter : public baseProductFilter
{
public:
    ProductTypeFilter(const TYPE type)
        : baseProductFilter([type](const Product& product)
        {
            return type == product.getInfo().type;
        })
    {
    }
};

class ProductDateFilter: public baseProductFilter
{
public:
    ProductDateFilter(const std::function<bool (Product)>& condition): baseProductFilter(condition){}
};

class ProductUnexpiredFilter: public ProductDateFilter
{
    public:
    ProductUnexpiredFilter(): ProductDateFilter([](Product product)
    {
        QDateTime now = QDateTime::currentDateTime();
        std::vector<LotInfo>& shipments = product.getShipments();
        for (auto shipment:shipments)
        {
            if (shipment.HSD > now) return true;
        }
        return false;
    }){}
};

class ProductExpiredFilter: public ProductDateFilter
{
    public:
    ProductExpiredFilter(): ProductDateFilter([](Product product)
    {
        QDateTime now = QDateTime::currentDateTime();
        std::vector<LotInfo>& shipments = product.getShipments();
        for (auto shipment:shipments)
        {
            if (shipment.HSD < now) return true;
        }
        return false;
    }){}
};

class ProductNearExpiryFilter: public ProductDateFilter
{
    public:
    ProductNearExpiryFilter(): ProductDateFilter([](Product product)
    {
        QDateTime from  = QDateTime::currentDateTime();
        QDateTime to = QDateTime::currentDateTime().addDays(15);
        std::vector<LotInfo>& shipments = product.getShipments();
        for (auto shipment:shipments)
        {
            if (from <= shipment.HSD && shipment.HSD <= to) return true;
        }
        return false;
    }){}
};

#endif //PRODUCTFILTER_H
