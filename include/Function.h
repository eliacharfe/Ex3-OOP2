#pragma once
#include "Utilities.h"

#include <string>

class Function
{
public:
    virtual ~Function() = default;
    virtual double operator()(double x) const = 0;
    virtual string to_string(const string& arg) const = 0;
};
