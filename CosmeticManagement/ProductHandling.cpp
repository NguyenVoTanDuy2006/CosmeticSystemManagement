#include "product management.h"

void productManagement::addProduct(const productInfo &info)
{
    products.push_back(Product(info));
}

void productManagement::removeProduct(string productID)
{
    for (auto& product : products)
    {
        if (product.ID  == productID)
        {
            if (productID != products.back().ID) swap(product, products.back());
            products.pop_back();
            sort(products.begin(), products.end());
            return;
        }
    }
}

Product& productManagement::findProduct(const string& ID)
{
    for (auto& product : products){
        if (product.ID == ID) return product;
    }
    return products[0];
}
