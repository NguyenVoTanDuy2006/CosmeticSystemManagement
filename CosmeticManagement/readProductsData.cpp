    #include "product management.h"

bool productManagement::readData()
{
    auto productsList = ifstream(getPath("ListProducts"));
    if (productsList.is_open() == false) { return false; }
    string ID;
    while (getline(productsList, ID) && ID != "null")
    {
        Product temp(ID);
        auto productData = ifstream(getPath(ID));
        if (!productData.is_open()) { return false; }
        productData >> temp;
        products.push_back(temp);
        productData.close();
    }
    Product::nextID = products.back().reverseID();
    productsList.close();
    return true;
}
