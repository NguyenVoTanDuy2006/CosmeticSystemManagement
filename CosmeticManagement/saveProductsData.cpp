#include  "product management.h"

bool productManagement::saveData() {

    removeProduct("");
    ofstream productList(getPath("ListProducts"));
    if (!productList.is_open()) return false;

    for (const auto& product : products) {
        productList << product.ID << '\n';
        ofstream productData(getPath(product.ID));
        productData << product;
        productData.close();
    }
    productList.close();
    return true;
}
