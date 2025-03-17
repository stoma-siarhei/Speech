#include <pch.hpp>

#include "com_des/com_des.hpp"
#include "python_impl/python_impl.hpp"


using namespace std;

using namespace ss::lib::python;

using namespace ss::lib::cl;

int main(int argc, char ** argv)
{
#ifdef _TEST
    // command line
    {
        string str{ " -script script -func foo -str Hello world -file D:\\test.wav" };
        try
        {
            deserialize des;
            des(str);
            cout << "-script -> " << des["-script"] << endl
                << "-func -> " << des["-func"] << endl
                << "-str -> " << des["-str"] << endl
                << "-file -> " << des["-file"] << endl;
            cout << des["-line"] << endl;
        }
        catch (out_of_range& e)
        {
            cout << e.what() << endl;
        }
        catch (...)
        {
            cout << "Unknow error" << endl;
        }
    }
    {
        try
        {
            deserialize des;
            des(argc, argv);
            cout << "-script -> " << des["-script"] << endl
                << "-func -> " << des["-func"] << endl
                << "-str -> " << des["-str"] << endl
                << "-file -> " << des["-file"] << endl;
            cout << des["-line"] << endl;
        }
        catch (out_of_range& e)
        {
            cout << e.what() << endl;
        }
        catch (...)
        {
            cout << "Unknow error" << endl;
        }
    }
    // python
    {
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
    }
#endif // _TEST
    try
    {
        deserialize des;
        des(argc, argv);
        string _script{ des["-script"] };
        executor exec(_script);
        string _func{ des["-func"] };
        string _str{ des["-str"] };
        string _file{ des["-file"] };
        exec(_func, _str, _file);
    }
    catch (out_of_range& e)
    {
        cout << e.what() << endl;
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
