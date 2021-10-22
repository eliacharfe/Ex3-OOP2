#pragma once

#include "Function.h"

#include <memory>

class Add : public Function
{
public:
    Add(const shared_ptr<Function>& arg1, const shared_ptr<Function>& arg2);
    double operator()(double x) const override;
    string to_string(const string& arg) const override;

private:
    const shared_ptr<Function> m_a;
    const shared_ptr<Function> m_b;
};
