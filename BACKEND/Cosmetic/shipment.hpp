#pragma once
#include "libs.hpp"

struct LotInfo
{
    QString id;
    QDateTime NSX, HSD;
    int quantity;
    LotInfo() : id(""), quantity(-1) {}
    LotInfo(
        const QDateTime &NSX,
        const QDateTime &HSD,
        const int &quantity)
    : NSX(NSX), HSD(HSD), quantity(quantity) { id = CreateLotID(); }

    static LotInfo& nullValue();

    void generateID();

    bool operator<(const LotInfo &a) const
    {
        return HSD < a.HSD;
    }
private:
    QString CreateLotID() {
        return NSX.toString("ddMMyyyy") + HSD.toString("ddMMyyyy");
    }
};

inline LotInfo& LotInfo::nullValue()
{
    static LotInfo nValue = LotInfo();
    return nValue;
}

inline void LotInfo::generateID()
{
    this->id = CreateLotID();
}

inline QTextStream &operator>>(QTextStream &is, LotInfo &obj)
{
    QString nsx_str, hsd_str;
    is >> nsx_str >> hsd_str >> obj.quantity;
    obj.NSX = QDateTime::fromString(nsx_str, "yyyy/MM/dd");
    obj.HSD = QDateTime::fromString(hsd_str, "yyyy/MM/dd");
    is.skipWhiteSpace();
    obj.generateID();
    return is;
}

inline QTextStream &operator<<(QTextStream &os, const LotInfo &obj)
{
    os << obj.NSX.toString("yyyy/MM/dd") << " "
       << obj.HSD.toString("yyyy/MM/dd") << " "
       << obj.quantity;
    return os;
}