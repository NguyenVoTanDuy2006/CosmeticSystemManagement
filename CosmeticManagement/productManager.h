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

    void addProduct(const productInfo& info);
    void removeProduct(std::vector<QString> IDs);
    Product& findProduct(const QString& name);

    std::vector<Product> getFilteredProducts(std::vector<std::shared_ptr<baseProductFilter>> conditions) const;

    bool importProduct(QString ID, const LotInfo& shipment);
    bool exportProduct(QString ID, int quantity);
    bool readData();
    bool saveData();
    std::vector<Product> getProducts() const {return products;}
};