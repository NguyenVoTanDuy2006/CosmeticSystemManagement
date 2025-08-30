#pragma once
#include "libs.hpp"

struct Client
{
    QString name;
    QString phoneNumber;
    QString address;
    Client(): name(""), phoneNumber(""), address("") {}
    Client(const QString& name, const QString& phoneNumber, const QString& address) : name(name), phoneNumber(phoneNumber), address(address) {}
};

inline QTextStream& operator >> (QTextStream& is, Client& client)
{
    client.name = is.readLine();
    client.phoneNumber = is.readLine();
    client.address = is.readLine();
    return is;
}

inline QTextStream& operator << (QTextStream& os, const Client& client)
{
    os << client.name << "\n" << client.phoneNumber << "\n" << client.address;
    return os;
}