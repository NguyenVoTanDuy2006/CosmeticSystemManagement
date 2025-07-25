#pragma once
#include <utility>

#include "libs.hpp"

enum class UNIT : int
{
    NONE = -1,
    CHAI = 1,
    TUYT = 2,
    HOP = 3,
};
enum class TYPE : int
{
    NONE = -1,
    SUA_RUA_MAT = 1,
    KEM = 2,
    SERUM = 3,
    SON = 4,
    NUOC = 5,
    MAT_NA = 6,
    NUOC_HOA = 7,
    GEL = 8,
    PHAN = 9,
};

struct productInfo
{
    string name, brand;
    TYPE type;
    UNIT unit;
    int price;
    productInfo() : name(""), brand(""), type(TYPE::NONE), unit(UNIT::NONE), price(-1) {};
    productInfo(string name, string brand, TYPE t, UNIT u, int price) : name(std::move(name)), brand(std::move(brand)), type(t), unit(u), price(price) {}
    productInfo(const productInfo& other);
};

inline productInfo::productInfo(const productInfo &other) {
    this->name = other.name;
    this->brand = other.brand;
    this->type = other.type;
    this->unit = other.unit;
    this->price = other.price;
}

inline istream &operator>>(istream &is, productInfo &ojb)
{
    getline(is, ojb.name);
    if (ojb.name == "null") return is;
    getline(is, ojb.brand);
    int type, unit, price;
    is >> type >> unit >> price;
    // is.ignore();
    ojb.type = static_cast<TYPE>(type);
    ojb.unit = static_cast<UNIT>(unit);
    ojb.price = price;
    return is;
}

inline ostream &operator<<(ostream &os, const productInfo &ojb)
{
    if (ojb.name == "")
        return (os << "null", os);
    os << ojb.name << '\n'
       << ojb.brand << '\n'
       << int(ojb.type) << " "
       << int(ojb.unit) << " "
       << ojb.price << '\n';
    return os;
}
