#pragma once

#include "libs.hpp"

struct Client
{
    string name;
    string phoneNumber;
    string address;
    Client(): name(""), phoneNumber(""), address("") {}
    Client(const string& name, const string& phoneNumber, const string& address) : name(name), phoneNumber(phoneNumber), address(address) {}
};

inline istream& operator >> (istream& is, Client& client)
{
    getline(is, client.name);
    getline(is, client.phoneNumber);
    getline(is, client.address);
    return is;
}

inline ostream& operator << (ostream& os, const Client& client)
{
    os << client.name << "\n" << client.phoneNumber << "\n" << client.address;
    return os;
}
