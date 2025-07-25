#include  "product management.h"

vector<Product> productManagement::filterByBrand(const string& brand) const
{
    vector<Product> ret = {};
    for (auto product : products){
        if (product.info.brand == brand)
        {
            ret.push_back(product);
        }
    }
    return ret;
}

vector<Product> productManagement::filterByType(const TYPE type) const
{
    vector<Product> ret = {};
    for (auto product : products)
    {
        if (product.info.type == type)
        {
            ret.push_back(product);
        }
    }
    return ret;
}
