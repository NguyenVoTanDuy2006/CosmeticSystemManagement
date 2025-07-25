#pragma once

#include "libs.hpp"
#include <ctime>
#include "tuple"

inline bool operator<(const tm &a, const tm &b)
{
    return tie(a.tm_year, a.tm_mon, a.tm_mday, a.tm_hour, a.tm_min, a.tm_sec) <
           tie(b.tm_year, b.tm_mon, b.tm_mday, b.tm_hour, b.tm_min, b.tm_sec);
}
inline bool operator==(const tm &a, const tm &b)
{
    return tie(a.tm_year, a.tm_mon, a.tm_mday, a.tm_hour, a.tm_min, a.tm_sec) ==
           tie(b.tm_year, b.tm_mon, b.tm_mday, b.tm_hour, b.tm_min, b.tm_sec);
}

inline istream &operator>>(istream &is, tm &ojb)
{
    const char *format = "%Y/%m/%d %H:%M:%S";
    ojb = {};
    is >> ws;
    is >> get_time(&ojb, format);
    mktime(&ojb);
    return is;
}

inline ostream &operator<<(ostream &os, const tm &ojb)
{
    os << setw(2) << setfill('0')
       << ojb.tm_year + 1900 << '/' << ojb.tm_mon + 1<< '/' << ojb.tm_mday << ' '
       << ojb.tm_hour << ':' << ojb.tm_min << ':' << ojb.tm_sec;
    return os;
}