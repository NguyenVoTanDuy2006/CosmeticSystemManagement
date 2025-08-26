//
// Created by nguye on 8/26/2025.
//


#include "product.hpp"

QString Product::getPicturePath(QString ID)
{
    const QString basePath = QDir::currentPath() + "Data/ProductData/Pics/";
    QDir dataPath(basePath);
    if (!dataPath.exists())
        dataPath.mkpath(".");
    return basePath + ID + ".jpg";
}
