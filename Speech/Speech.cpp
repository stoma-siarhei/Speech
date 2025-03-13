﻿#include <pch.hpp>

#include "com_des/com_des.hpp"
#include "python_impl/python_impl.hpp"


using namespace std;

using namespace ss::lib::python;

using namespace ss::lib::cl;

int main(int argc, char ** argv)
{
    deserialize des;
    des(argc, argv);
    try
    { 
        executor exec{ "script" };
        cout << exec("foo", 1, 2, 3) << endl;
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
