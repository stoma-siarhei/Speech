#include <pch.hpp>

#include "python_impl/python_impl.hpp"


using namespace std;

using namespace ss::lib::python;

int main(int argc, char ** argv)
{
    try
    { 
        executor exec{ "script" };
        exec("foo", 1, 2, 3);
    }
    catch (runtime_error& e)
    { 
        cout << e.what() << endl;
    }
    catch (...)
    { 
        cout << "Unknow error" << endl;
    }
    return 0;
}
