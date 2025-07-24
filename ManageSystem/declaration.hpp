#pragma once

#include "libs.hpp"

struct client
{
    string name;
    string PhoneNumber;
    string Address;
    client(string name, string PhoneNumber, string Address) : name(name), PhoneNumber(PhoneNumber), Address(Address) {}
};