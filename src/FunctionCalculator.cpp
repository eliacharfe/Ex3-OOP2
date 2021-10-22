#include "FunctionCalculator.h"

#include "Sin.h"
#include "Ln.h"
#include "Poly.h"
#include "Mul.h"
#include "Add.h"
#include "Comp.h"
#include "Log.h"

#include <istream>
#include <ostream>
#include <iomanip>
#include <sstream>
#include <iostream>

FunctionCalculator::FunctionCalculator(istream& istr, ostream& ostr)
    : m_actions(createActions()), m_functions(createFunctions()), m_istr(istr), m_ostr(ostr), m_line(0)
{
    do {
        try {
            setMaxFunction(m_istr);
            m_running = false;
        }
        catch (std::invalid_argument& e) { cerr << e.what() << '\n'; }
        catch (std::out_of_range& e)     { cerr << e.what() << '\n'; }
        catch (std::exception& e)        { cerr << e.what() << '\n'; }
    } while (m_running);
    m_running = true;
}
//-------------------------------------
void FunctionCalculator::run(istream& input) 
{
    m_ostr << std::setprecision(2) << std::fixed;
    do  {
        try {
            ++m_line;
            m_ostr << '\n';
            printFunctions();
            m_ostr << "Enter command ('help' for the list of available commands): ";
            const auto action = readAction(input);
            runAction(action, input);
        }
        catch (MyException& e) { cerr << e.what() << '\n';
            if (m_readState) // if in file mode reading --> throw exception
                throw MyException2("\nError at the file\n: My Exception 2 --> My Exception\n"); 
            clear(input); }
        catch (std::invalid_argument& e) { cerr << e.what() << '\n';
            if (m_readState) // if in file mode reading --> throw exception
                throw MyException2("\nError at the file\n: My Exception 2 --> invalid argument\n");
            clear(input);
        }
        catch (std::istream::failure& e) {  cerr << e.what() << '\n';
            if (m_readState)// if in file mode reading --> throw exception
                throw MyException2("\nError at the file\n: My Exception 2 --> istream failure\n");
            clear(input);
        }
        catch (std::out_of_range& e)  { cerr << e.what() << '\n'; clear(input); }
    } while (m_running);
}
//-----------------------------------------------
void FunctionCalculator::eval(istream& input)
{
    if (auto i = readFunctionIndex(input); i)
    {
        auto x = 0.;
        input >> x;
        if (input.fail() || input.bad())  // throw exception
            throw std::istream::failure("\neval: the number is not a double\n: istream failure\n");
        
        auto sstr = std::ostringstream();
        sstr << std::setprecision(2) << std::fixed << x;
        m_ostr << m_functions[*i]->to_string(sstr.str())  << " = " << (*m_functions[*i])(x) << '\n';
    }
}
//-------------------------------------------
void FunctionCalculator::poly(istream& input)
{
    auto n = 0;
    input >> n;
    if (input.fail())  // throw exception
        throw std::istream::failure("\npoly: input failed. need integer\n: istream failure\n");
    if (n < 0) // throw exception
        throw std::invalid_argument("\npoly: number of coefficients is negative\n: invalid_argument\n");

    auto coeffs = vector < double >(n);
    for (auto& coeff : coeffs) {
        input >> coeff;
        if (input.fail()) // throw exception
            throw std::istream::failure("\npoly: input failed. coefficient isn't a double\n: istream failure\n");
    }
    checkNumFunctions();
    m_functions.push_back(make_shared < Poly >(coeffs));
}
//-----------------------------------------
void FunctionCalculator::log(istream& input)
{
    auto base = 0;
    input >> base;
    if (input.fail()) // throw exception
        throw std::istream::failure("\nlog: input failed\n: istream failure\n");
    if (base < 2)// throw exception
        throw std::invalid_argument("\nlog: base can't be negative or 0, or 1\n: invalid_argument\n");

    checkNumFunctions();

    if (auto f = readFunctionIndex(input); f)
        m_functions.push_back(make_shared < Log >(base, m_functions[*f]));
}
//---------------------------------------
void FunctionCalculator::del(istream& input)
{
    if (auto i = readFunctionIndex(input); i)
        m_functions.erase(m_functions.begin() + *i);
}
//-----------------------------------------------
void FunctionCalculator::help()
{
    m_ostr << "The available commands are:\n";
    for (const auto& action : m_actions)
        m_ostr << "* " << action.command << action.description << '\n'; 
    m_ostr << '\n';
}
//--------------------------------------------
void FunctionCalculator::exit()
{
    m_ostr << "Goodbye!\n";
    m_running = false;
}
//------------------------------------------------
void FunctionCalculator::printFunctions() const
{
    m_ostr << "List of available gates:\n";
    for (decltype(m_functions.size()) i = 0; i < m_functions.size(); ++i)
        m_ostr << i << ".\t" << m_functions[i]->to_string("x") << '\n';
    m_ostr << '\n';
}
//-------------------------------------------------------------
std::optional< int > FunctionCalculator::readFunctionIndex(istream& input) const
{
    auto i = 0;
    input >> i;
    if (input.fail()) // throw exception
        throw std::istream::failure("\nfunc error: input failed\n: istream failure\n");
    if (i < 0) // throw exception
        throw std::invalid_argument("\nfunc error: number of function can't be negative\n: invalid_argument\n");
    if (i >= m_functions.size())
    {
        m_ostr << "\nFunction #" << i << " doesn't exist\n";
        throw MyException("\nfunction number dosn't exist\n: My Exception");// throw exception
        return {};
    }
    return i;
}
//---------------------------------------------------------------
FunctionCalculator::Action FunctionCalculator::readAction(istream& input) const
{
    auto action = string();
    input >> action;
    if (input.fail())// throw exception
        throw std::istream::failure("\naction failed: input failed\n: istream failure\n");

    for (auto& c : action)  {
        if (isdigit(c))// throw exception
            throw std::invalid_argument("\naction failed: name of function contains digits\n: invalid_argument\n");
    }
    for (decltype(m_actions.size()) i = 0; i < m_actions.size(); ++i)
        if (action == m_actions[i].command)
            return m_actions[i].action;

    return Action::Invalid;
}
//-------------------------------------------------
void FunctionCalculator::runAction(Action action, istream& input)
{
    switch (action)
    {
        default: m_ostr << "Unknown enum entry used!\n";  break;
    case Action::Invalid: { m_ostr << "\nCommand not found\n"; 
        throw std::invalid_argument(": invalid argument (name of function)");// throw exception
        break;
    }
        case Action::Eval: eval(input);                break;
        case Action::Poly: poly(input);                break;
        case Action::Mul:  binaryFunc < Mul >(input);  break;
        case Action::Add:  binaryFunc < Add >(input);  break;
        case Action::Comp: binaryFunc < Comp >(input); break;
        case Action::Log:  log(input);                 break;
        case Action::Del:  del(input);                 break;
        case Action::Help: help();                     break;
        case Action::Exit: exit();                     break;
        case Action::Read: read();                     break;
    }
}
//---------------------------------------------------------------
FunctionCalculator::ActionMap FunctionCalculator::createActions()
{
    return ActionMap
    {
        {  "eval", "(uate) num x - compute the result of function #num on x",
            Action::Eval
        },
        {  "poly", "(nomial) N c_0 c_1 ... c_(N-1) - creates a polynomial with N coefficients",
            Action::Poly
        },
        {  "mul", "(tiply) num1 num2 - Creates a function that is the multiplication of function #num1 and function #num2",
            Action::Mul
        },
        {  "add", " num1 num2 - Creates a function that is the sum of function #num1 and function #num2",
            Action::Add
        },
        {  "comp", "(osite) num1 num2 - creates a function that is the composition of function #num1 and function #num2",
            Action::Comp
        },
        {  "log", " N num - create a function that is the log_N of function #num",
            Action::Log
        },
        {  "del", "(ete) num - delete function #num from the function list",
            Action::Del
        },
        {  "help", " - print this command list",
            Action::Help
        },
        {  "exit", " - exit the program",
            Action::Exit
        },
        {  "read"," - read input from a text file",
            Action::Read
        }
    };
}
//-----------------------------------------------------------------------
FunctionCalculator::FunctionList FunctionCalculator::createFunctions()
{
    return FunctionList {
        make_shared < Sin >(),
        make_shared < Ln >()
    };
}
//---------------------------------------
void FunctionCalculator::read()
{   
    ifstream input;
    openFile(input);
    m_readState = true;
    m_line = 0;
    while (!input.eof() && m_read)
    {
        try { 
            run(input); // run commands from the file
        }
        catch (MyException2& e)  { 
            clear(input);
            cerr << e.what() << '\n';
            cout << "Error at line: " << m_line << "\nDo you want to continue reading from the file? 'Y'/'N'?\n";
            char c;
            cin >> c;
            m_read = checkReadingState(c); // if return false --> break (stop reading from file)
        }
    }
    m_read = true;
    m_readState = false;
    m_line = 0;
    input.close();
}
//-------------------------------------------
bool FunctionCalculator::checkReadingState(char c) 
{
    while (true) { 
        try {
            if (c == 'Y' || c == 'y') {
                clear(cin);
                return true;
            }
            else if (c == 'N' || c == 'n') {
                clear(cin);
                return false;
            }
            else { // throw exception 3
                clear(cin);
                throw MyException3(); // bad input from user
            }
        }
        catch (MyException3) { // catch exception 3
            m_ostr << "read: Bad input, Try Again\n: istream failure --> Exception 3\n\nDo you want to continue reading from the file? 'Y'/'N'?\n";
            cin >> c; // try again
        }
    }
}
//----------------------------------------------------
void FunctionCalculator::openFile(ifstream& input) 
{
    m_ostr << "Enter a file name with .txt:\n";
    string nameFile;
    cin >> nameFile;
    input.open(nameFile);
    if (!input.is_open())// throw exception
        throw MyException("Cannot open file\n: My Exception\n");
}
//------------------------------------------------------------
void FunctionCalculator::setMaxFunction(istream& input)
{
    m_ostr << "Enter max of functions:\n";
    input >> m_maxFunc;
    if (input.fail())  {
        clear(input);// throw exception
        throw std::invalid_argument("max error: Your input is not an integer\n: invalid_argument\n");
    }
    if (m_maxFunc > MAX || m_maxFunc < 2)// throw exception
        throw std::out_of_range("max error: max function should be between 2 and 100\n: out of range\n");
}
//----------------------------------------------
void FunctionCalculator::checkNumFunctions() const
{
    if (m_functions.size() >= m_maxFunc)
        throw std::out_of_range("too many functions\n: out of range\n");// throw exception
}
//------------------------------------------------------
void FunctionCalculator::clear(istream& input)
{
    input.clear();
    input.ignore(std::numeric_limits < std::streamsize >::max(), '\n');
}