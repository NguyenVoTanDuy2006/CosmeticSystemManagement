#include "product.hpp"

string Product::CreateID(int number)
{
    stringstream IDstream;
    IDstream << "SP" << setw(5) << setfill('0') << number;
    return IDstream.str();
}

string Product::CreateID()
{
    stringstream IDstream;
    IDstream << "SP" << setw(5) << setfill('0') << nextID;
    nextID++;
    return IDstream.str();
}
