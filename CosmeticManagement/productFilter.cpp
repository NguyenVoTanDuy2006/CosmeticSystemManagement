#include "productManager.h"

std::vector<Product> productManager::getFilter(std::vector<std::shared_ptr<baseProductFilter>> conditions) const
{
    std::vector<Product> products;
    for (const auto& item : this->products)
    {
        bool flag = true;
        for (const auto& condition: conditions)
        {
            if (condition->check(item) == false)
            {
                flag = false;
                break;
            }
        }
        if (flag == true) products.push_back(item);
    }
    return products;
}