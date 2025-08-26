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


#endif //PRODUCTFILTER_H
