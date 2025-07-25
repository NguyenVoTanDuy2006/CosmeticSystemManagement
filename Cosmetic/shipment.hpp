#pragma once

#include "libs.hpp"
#include "time.hpp"

struct LotInfo // so lo
{
    string id;
    tm NSX, HSD;
    int quantity;
    LotInfo() : id(""), NSX(), HSD(), quantity(-1) {}
    LotInfo(const tm &NSX, const tm &HSD, const int &quantity) : NSX(NSX), HSD(HSD), quantity(quantity) { id = CreateLotID(); }

    bool operator<(const LotInfo &a) const
    {
        return HSD < a.HSD;
    }
    string CreateLotID()
    {
        stringstream ID;
        ID << setw(2) << setfill('0')
           << NSX.tm_mday << NSX.tm_mon << NSX.tm_year
           << HSD.tm_mday << HSD.tm_mon << HSD.tm_year;
        return ID.str();
    }
};

inline istream &operator>>(istream &is, LotInfo &ojb)
{
    ojb.NSX = {};
    ojb.HSD = {};
    is >> ojb.NSX >> ojb.HSD >>ojb.quantity;
    return is;
}

inline ostream &operator<<(ostream &os, const LotInfo &ojb)
{
    os << ojb.NSX << " " << ojb.HSD << " " << ojb.quantity;
    return os;
}