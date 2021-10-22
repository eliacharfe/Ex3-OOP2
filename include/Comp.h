#pragma once

#include "Function.h"

#include <memory>

class Comp : public Function
{
public:
    Comp(const shared_ptr<Function>& arg1, const std::shared_ptr<Function>& arg2);
    double operator()(double x) const override;
    string to_string(const std::string& arg) const override;

private:
    const shared_ptr<Function> m_a;
    const shared_ptr<Function> m_b;
};