#include "product.hpp"

string product::CreateID(int number)
{
    stringstream IDstream;
    IDstream << "SP" << setw(5) << setfill('0') << number;
    return IDstream.str();
}
string product::CreateID()
{
    stringstream IDstream;
    IDstream << "SP" << setw(5) << setfill('0') << nextID;
    nextID++;
    return IDstream.str();
}