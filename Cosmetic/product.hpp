#pragma once
#include "libs.hpp"
#include "productInfo.hpp"
#include "shipment.hpp"

class product
{
private:
    static int nextID;
    string ID;
    productInfo info;

    vector<LotInfo> Shipments;
    void addShipment(const LotInfo &lot);
    void removeShipment(LotInfo &lot);
    LotInfo &findShipment(const tm &date);

    static string CreateID();
    static  string CreateID(int number);

public:
    product();
    product(int id, const productInfo &info);
    product(const string &ID, const productInfo &info);
    explicit product(const productInfo &info);
    explicit product(const string& ID);
    ~product();

    product &operator=(const product &other);

    static void setLastIDNumber(int number);

    productInfo getInfo();

    friend istream &operator>>(istream &is, product &ojb);
    friend ostream &operator<<(ostream &os, const product &ojb);
};
