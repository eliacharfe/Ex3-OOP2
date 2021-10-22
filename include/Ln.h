#pragma once

#include "Function.h"

class Ln : public Function
{
public:
    double operator()(double x) const override;
    string to_string(const string& arg) const override;
};
