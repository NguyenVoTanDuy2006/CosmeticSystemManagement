#pragma once

#include  "libs.hpp"
#include "product.hpp"

class productManagement
{
private:
    vector<Product> products;
    static string getPath(const string& name);

public:
    productManagement();
    ~productManagement();

    //product handling
    void addProduct(const productInfo& info);
    void removeProduct(string ID);
    Product& findProduct(const string& ID);

    //filter
    vector<Product> filterByBrand(const string& brand) const;
    vector<Product> filterByType(TYPE type) const;

    //test for check if code not working
    void displayAll()
    {
        for (auto product : products)
        {
            cout << product << '\n'
                << "-------------------------------------------\n";
        }
    }

    //data handling
    bool readData();
    bool saveData();
};
