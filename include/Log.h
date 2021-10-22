#pragma once

#include "Function.h"

#include <memory>

class Log : public Function
{
public:
    Log(int base, const std::shared_ptr<Function>& func);
    double operator()(double x) const override;
    string to_string(const string& arg) const override;

private:
    int m_base;
    const shared_ptr<Function> m_func;
};
