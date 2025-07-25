#pragma once
#include "libs.hpp"
#include "productInfo.hpp"
#include "shipment.hpp"

class Product
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
    Product();
    Product(int id, const productInfo &info);
    Product(const string &ID, const productInfo &info);
    explicit Product(const productInfo &info);
    explicit Product(const string& ID);
    ~Product();

    Product &operator=(const Product &other);
    bool operator <= (const Product &other) const;

    static void setLastIDNumber(int number);
    productInfo& getInfo();


    friend istream &operator>>(istream &is, Product &ojb);
    friend ostream &operator<<(ostream &os, const Product &ojb);

    friend class productManagement;
};
