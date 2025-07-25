#include "product.hpp"

int Product::nextID = 1;

Product::Product()
{
    ID = "";
    info = productInfo();
    Shipments = {};
}

Product::Product(const productInfo &info) : info(info), ID(CreateID()), Shipments({}){}

Product::Product(const string &ID): ID(ID) {}

Product::Product(int id, const productInfo &info)
{
    this->ID = CreateID(id);
    this->info = info;
}

Product::Product(const string &ID, const productInfo &info) : ID(ID), info(info) {}

Product::~Product()
{
    Shipments.clear();
}

Product &Product::operator=(const Product &other)
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

bool Product::operator<=(const Product& other) const
{
    return this->ID <= other.ID;
}

bool Product::operator<(const Product& other) const
{
    return this->ID < other.ID;
}

int Product::reverseID() const
{
    stringstream IDstream (this->ID);
    char S, P;
    IDstream>> S >> P;
    int number; IDstream>> number;
    return number;
}

void Product::setLastIDNumber(int number) {nextID = number;}

productInfo& Product::getInfo()
{
    return this->info;
}
