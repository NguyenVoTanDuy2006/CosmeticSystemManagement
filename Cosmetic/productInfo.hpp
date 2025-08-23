#pragma once
#include "libs.hpp"

enum class UNIT : int
{
    NONE = -1, CHAI = 1, TUYT = 2, HOP = 3,
};
enum class TYPE : int
{
    NONE = -1, SUA_RUA_MAT = 1, KEM = 2, SERUM = 3, SON = 4,
    NUOC = 5, MAT_NA = 6, NUOC_HOA = 7, GEL = 8, PHAN = 9,
};

struct productInfo
{
    QString name, brand;
    TYPE type;
    UNIT unit;
    int price;
    productInfo() : name(""), brand(""), type(TYPE::NONE), unit(UNIT::NONE), price(-1) {};
    productInfo(QString name, QString brand, TYPE t, UNIT u, int price) : name(std::move(name)), brand(std::move(brand)), type(t), unit(u), price(price) {}
    productInfo(const productInfo& other);
};

inline productInfo::productInfo(const productInfo &other) {
    this->name = other.name;
    this->brand = other.brand;
    this->type = other.type;
    this->unit = other.unit;
    this->price = other.price;
}

inline QTextStream &operator>>(QTextStream &is, productInfo &obj)
{
    obj.name = is.readLine();
    if (obj.name == "null") return is;
    obj.brand = is.readLine();
    int type, unit, price;
    is >> type >> unit >> price;
    is.skipWhiteSpace();
    obj.type = static_cast<TYPE>(type);
    obj.unit = static_cast<UNIT>(unit);
    obj.price = price;
    return is;
}

inline QTextStream &operator<<(QTextStream &os, const productInfo &obj)
{
    if (obj.name.isEmpty()) {
        os << "null";
        return os;
    }
    os << obj.name << '\n'
       << obj.brand << '\n'
       << static_cast<int>(obj.type) << " "
       << static_cast<int>(obj.unit) << " "
       << obj.price << '\n';
    return os;
}