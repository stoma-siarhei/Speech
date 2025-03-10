#include <pch.hpp>

#include "python_impl/python_impl.hpp"


using namespace std;

int main(int argc, char ** argv)
{
    ss::lib::python::executor exec{ "script" };
    exec("foo", 1, 2, 3);
    return 0;
}
