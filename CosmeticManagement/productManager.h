#pragma once
#include "libs.hpp"
#include "product.hpp"
#include "productFilter.h"


class productManager
{
private:
    std::vector<Product> products;
    static QString getPath(const QString& name);
    productManager();

public:
    static productManager& getInstance();
    ~productManager();

    productManager(const productManager&) = delete;
    productManager& operator=(const productManager&) = delete;

    void addProduct(const productInfo& info);
    void removeProduct(std::vector<QString> IDs);
    Product& findProductByName(const QString& name);
    Product& findProductByID(const QString& ID);

    std::vector<Product> getFilteredProducts(std::vector<std::shared_ptr<baseProductFilter>> conditions) const;

    QString NameToID(const QString& name) const;

    bool importProduct(QString ID, const LotInfo& shipment);
    bool exportProduct(QString ID, int quantity);
    bool readData();
    bool saveData();
    std::vector<Product> getProducts() const {return products;}
};
