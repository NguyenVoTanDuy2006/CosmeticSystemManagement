#include "productManager.h"

std::vector<Product> productManager::getFilteredProducts(std::vector<std::shared_ptr<baseProductFilter>> conditions) const
{
    std::vector<Product> filteredProducts;
    for (const auto& item : this->products)
    {
        for (const auto& condition: conditions)
        {
            if (condition->check(item))
            {
                filteredProducts.push_back(item);
                break;
            }
        }
    }
    return filteredProducts;
}