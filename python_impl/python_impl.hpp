#ifndef _PYTHON_IMPL_HPP_
#define _PYTHON_IMPL_HPP_


#include <pch.hpp>


using namespace std;

namespace ss::lib::python
{

using object_t = PyObject;

using object_ptr = PyObject*;

struct decoder
{
	decoder() = delete;

	decoder(const char* str);

	decoder(const string& str);

	decoder(const string_view str);

	decoder(const object_ptr p_obj, const char* str);

	decoder(const object_ptr p_obj, const string& str);

	decoder(const object_ptr p_obj, const string_view str);

	~decoder();

	const object_ptr operator()() const noexcept;
private:
	object_ptr m_value;
}; // struct decoder

class executor
{
public:
	executor();

	executor(const char* name);

	executor(const string& name);

	executor(const string_view name);

	~executor();

protected:
	bool load_module(const object_ptr name) noexcept;

	bool load_func(const object_ptr func) const noexcept;
private:
	object_ptr m_module{ nullptr };
}; // class executor

} // namespace ss::lib::python


#endif // _PYTHON_IMPL_HPP_
