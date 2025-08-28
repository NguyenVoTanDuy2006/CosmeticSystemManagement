#pragma once
#include "libs.hpp"
#include "productInfo.hpp"
#include "shipment.hpp"

class Product
{
private:
    static int nextID;
    QString ID;
    productInfo info;
    QString picture;
    std::vector<LotInfo> Shipments;

    void addShipment(const LotInfo &lot);
    void removeShipment(LotInfo &lot);
    LotInfo &findShipment(const QString& ID);

    static QString getPicturePath(QString ID);
    static QString CreateID();
    static QString CreateID(int number);

public:
    int reverseID() const; // Chuyển sang public để readData có thể truy cập
    Product();
    Product(int id, const productInfo &info);
    Product(const QString &ID, const productInfo &info);
    explicit Product(const QString& ID);
    explicit Product(const productInfo &info);
    explicit Product(const productInfo &info, const QString& pictureSourcePath);

    ~Product();

    Product &operator=(const Product &other);
    bool operator <= (const Product &other) const;
    bool operator < (const Product &other) const;

    static void setLastIDNumber(int number);

    const QString& getID() const;
    const QString& getPicturePath() const;
    productInfo getInfo() const;
    QString getHSD() const;
    int getStock() const;
    std::vector<LotInfo>& getShipments();


    friend QTextStream &operator>>(QTextStream &is, Product &obj);
    friend QTextStream &operator<<(QTextStream &os, const Product &obj);

    friend class productManager;
};