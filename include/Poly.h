#pragma once

#include "Function.h"

#include <vector>

class Poly : public Function
{
public:
    Poly(const vector<double>& coeffs);
    double operator()(double x) const override;
    string to_string(const string& arg) const override;

private:
    vector<double> m_coeffs;
};
