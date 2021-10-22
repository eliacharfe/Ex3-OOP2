#pragma once
#include "Utilities.h"
#include "MyException.h"

#include <vector>
#include <memory>
#include <string>
#include <iosfwd>
#include <optional>
#include <fstream>

class Function;

class FunctionCalculator
{
public:
    FunctionCalculator(istream& istr, ostream& ostr);
    void run(istream& input);

private:
    void setMaxFunction(istream& input);
    void checkNumFunctions() const;
    void clear(istream& input);
    void openFile(ifstream& input);
    bool checkReadingState(char c) ;

    void eval(istream& input);
    void poly(istream& input);
    void log(istream& input);
    void del(istream& input);
    void help();
    void exit();
    void read();

    template <typename FuncType>
    void binaryFunc(istream& input)
    {
        if (auto f0 = readFunctionIndex(input), f1 = readFunctionIndex(input); f0 && f1)
        {
            checkNumFunctions();
            m_functions.push_back(make_shared< FuncType >(m_functions[*f0], m_functions[*f1]));
        }
    }


    void printFunctions() const;

    enum class Action
    {
        Invalid,
        Eval,
        Poly,
        Mul,
        Add,
        Comp,
        Log,
        Del,
        Help,
        Exit,
        Read,
    };

    struct ActionDetails
    {
        string command;
        string description;
        Action action;
    };

    using ActionMap = vector < ActionDetails >;
    using FunctionList = vector < shared_ptr < Function >>;

    const ActionMap m_actions;
    FunctionList m_functions;
    bool m_running = true;
    istream& m_istr;
    ostream& m_ostr;

    std::optional<int> readFunctionIndex(istream& input) const;
    Action readAction(istream& input) const;
    void runAction(Action action, istream& input);

    static ActionMap createActions();
    static FunctionList createFunctions();

    int m_maxFunc;
    int m_line;
    bool m_read = true;
    bool m_readState = false;
};
