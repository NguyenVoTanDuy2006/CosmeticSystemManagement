#include "product.hpp"

#include <qtestsupport_core.h>

int Product::nextID = 1;

Product::Product() {
    ID = "";
    info = productInfo();
    Shipments = {};
}

Product::Product(const QString &ID): ID(ID), picture(getPicturePath(ID)) {}

Product::Product(const productInfo &info) : info(info), ID(CreateID()), Shipments({}){}

Product::Product(const productInfo& info, const QString& pictureSourcePath): Product(info)
{
    const QString desPath = getPicturePath(this->ID);
    QImage image(pictureSourcePath);
    if (!image.isNull())
    {
        image.save(desPath, "JPG", -1);
    }
    picture = desPath;
}

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

const QString& Product::getID() const { return ID; }

const QString& Product::getPicturePath() const
{
    return this->picture;
}

int Product::getStock() const {
    int total = 0;
    for (const auto& lot : Shipments) total += lot.quantity;
    return total;
}

QString Product::getHSD() const {
    if (Shipments.empty()) return "";
    return QString(Shipments.front().HSD.toString("dd/MM/yyyy"));
}