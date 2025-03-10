#include "python_impl.hpp"


using namespace std;

namespace ss::lib::python
{

decoder::decoder(const char* str)
{
	m_value = PyUnicode_DecodeFSDefault(str);
}

decoder::decoder(const string& str) 
	: decoder(str.data())
{
}

decoder::decoder(const string_view str) 
	: decoder(str.data())
{
}

decoder::decoder(const object_ptr p_obj, const char* str)
{
	m_value = PyObject_GetAttrString(p_obj, str);
}

decoder::decoder(const object_ptr p_obj, const string& str) 
	: decoder(p_obj, str.data())
{
}

decoder::decoder(const object_ptr p_obj, const string_view str)
	: decoder(p_obj, str.data())
{
}

decoder::~decoder()
{
	Py_DECREF(m_value);
}

const object_ptr decoder::operator()() const noexcept
{
	return m_value;
}

executor::executor()
{
	Py_Initialize();
}

executor::executor(const char* name) 
	: executor()
{
	if (!load_module(decoder(name)()))
	{
		string str = "Error load script - ";
		str += name;
		throw runtime_error(str);
	}
}

executor::executor(const string& name) 
	: executor(name.data())
{
}

executor::executor(const string_view name) 
	: executor(name.data())
{
}

executor::~executor()
{
	if (m_module != nullptr) Py_DECREF(m_module);

	Py_FinalizeEx();
}

bool executor::load_module(const object_ptr name) noexcept
{
	m_module = PyImport_Import(name);

	return m_module != nullptr;
}

bool executor::load_func(const object_ptr func) const noexcept
{
	return PyCallable_Check(func);
}

} // ss::lib::python
