#include <iostream>
#include "string.h"

std::ostream& operator << (std::ostream &out, const string &str) {
    out << str.str ;
    return out ;
}