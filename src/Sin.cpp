#include "Sin.h"

#include <cmath>

double Sin::operator()(double x) const
{
    return std::sin(x);
}

string Sin::to_string(const string& arg) const
{
    return "sin(" + arg + ")";
}
