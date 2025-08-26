#pragma once
#include "libs.hpp"
#include "product.hpp"
#include "productFilter.h"


class productManager
{
private:
    std::vector<Product> products;
    static QString getPath(const QString& name);

public:
    productManager();
    ~productManager();

    void addProduct(const productInfo& info);
    void removeProduct(std::vector<QString> IDs);
    Product& findProduct(const QString& name);

    std::vector<Product> getFilter(std::vector<std::shared_ptr<baseProductFilter>> conditions) const;

    void displayAll() const {
        qDebug() << "--- ALL PRODUCTS ---";
        for (const auto& product : products) {
             if (product.getInfo().name.isEmpty()) continue;
             QString productString;
             QTextStream ss(&productString);
             ss << product;
             qDebug().noquote() << productString;
             qDebug() << "-------------------------------------------";
        }
    }
    std::vector<Product> getProducts() const {return products;}
    bool readData();
    bool saveData();
};