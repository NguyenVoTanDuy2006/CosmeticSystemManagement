#pragma once
#include "libs.hpp"
#include "product.hpp"

class productManager
{
private:
    std::vector<Product> products;
    static QString getPath(const QString& name);

public:
    productManager();
    ~productManager();

    void addProduct(const productInfo& info);
    void removeProduct(const QString& ID);
    Product& findProduct(const QString& name);

    std::vector<Product> filterByBrand(const QString& brand) const;
    std::vector<Product> filterByType(TYPE type) const;

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

    bool readData();
    bool saveData();
};