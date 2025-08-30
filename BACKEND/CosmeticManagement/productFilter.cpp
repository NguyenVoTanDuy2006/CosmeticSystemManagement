#include "productManager.h"

std::vector<Product> productManager::getFilteredProducts(std::vector<std::shared_ptr<baseProductFilter>> conditions) const
{
    std::vector<Product> filteredProducts;
    for (const auto& item : this->products)
    {
        if (std::all_of(conditions.begin(), conditions.end(),
                [&](const auto& condition) {return condition->check(item);}))
        {
            filteredProducts.push_back(item);
        }
    }
    return filteredProducts;
}