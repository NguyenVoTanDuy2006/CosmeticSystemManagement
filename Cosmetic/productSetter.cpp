#include "product.hpp"
//
// Created by nguye on 8/29/2025.
//
void Product::setInfo(const productInfo& info)
{
    this->info = info;
}

void Product::setPicture(const QString& sourcePath)
{
    QFile::remove(this->picture);
    const QString desPath = getPicturePath(this->ID);
    QImage image(sourcePath);
    if (!image.isNull())
    {
        image.save(desPath, "JPG", -1);
    }
    this->picture = desPath;
}
