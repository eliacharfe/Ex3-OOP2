#pragma once

#include "Function.h"

class Sin : public Function
{
public:
    double operator()(double x) const override;
    string to_string(const std::string& arg) const override;
};
