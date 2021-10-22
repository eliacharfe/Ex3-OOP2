#include "FunctionCalculator.h"
#include <iostream>

int main()
{
    try {
        FunctionCalculator(cin, cout).run(cin);
    }
    catch (std::exception& e) { cerr << e.what() << '\n'; }
}
