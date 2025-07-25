#include  "product management.h"

string productManagement::getPath(const string& name)
{
    return "Data/ProductData/" + name + ".txt";
}

productManagement::productManagement() {
    products.emplace_back();          // dummy product
}

productManagement::~productManagement() {
    products.clear();
}


