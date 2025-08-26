#include "product.hpp"

int Product::nextID = 1;

Product::Product() {
    ID = "";
    info = productInfo();
    Shipments = {};
}

Product::Product(const productInfo &info) : info(info), ID(CreateID()), Shipments({}){}

Product::Product(const QString &ID): ID(ID) {}

Product::Product(int id, const productInfo &info) {
    this->ID = CreateID(id);
    this->info = info;
}

Product::Product(const QString &ID, const productInfo &info) : ID(ID), info(info) {}

Product::~Product() {
    Shipments.clear();
}

Product &Product::operator=(const Product &other) {
    if (this == &other)
        return *this;

    this->ID = other.ID;
    this->info = other.info;
    this->Shipments = other.Shipments; // std::vector gán hiệu quả
    return *this;
}

bool Product::operator<=(const Product& other) const {
    return this->ID <= other.ID;
}

bool Product::operator<(const Product& other) const {
    return this->ID < other.ID;
}

int Product::reverseID() const {
    if (this->ID.startsWith("SP")) {
        bool ok;
        int number = this->ID.right(5).toInt(&ok);
        if (ok) return number;
    }
    return 0; // Trả về 0 nếu có lỗi
}

void Product::setLastIDNumber(int number) {
    nextID = number;
}

productInfo Product::getInfo() const{
    return this->info;
}

const std::string& Product::getID() const { return ID; }

int Product::getStock() const {
    int total = 0;
    for (const auto& lot : Shipments) total += lot.quantity;
    return total;
}

std::string Product::getHSD() const {
    if (Shipments.empty()) return "";
    return Shipments.front().HSD.toString("dd/MM/yyyy").toStdString();
}