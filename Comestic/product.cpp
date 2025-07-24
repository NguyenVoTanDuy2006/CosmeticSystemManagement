#include "product.hpp"

int product::nextID = 1;

product::product()
{
    ID = "";
    info = productInfo();
    Shipments = {};
}

product::product(const productInfo &info) : info(info), ID(CreateID()), Shipments({}){}

product::product(const string &ID): ID(ID) {}

product::product(int id, const productInfo &info)
{
    this->ID = CreateID(id);
    this->info = info;
}

product::product(const string &ID, const productInfo &info) : ID(ID), info(info) {}

product::~product()
{
    Shipments.clear();
}

product &product::operator=(const product &other)
{
    if (this == &other)
        return *this;

    this->ID = other.ID;
    this->info = other.info;
    this->Shipments.clear();
    for (auto item : other.Shipments)
    {
        this->Shipments.push_back(item);
    }
    return *this;
}

void product::setLastIDNumber(int number) {nextID = number;}

productInfo product::getInfo()
{
    return this->info;
}
