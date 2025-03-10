#ifndef _PYTHON_IMPL_HPP_
#define _PYTHON_IMPL_HPP_


#include <pch.hpp>

#include <include/Python.h>


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
	object_ptr m_value{ nullptr };
}; // struct decoder

class executor
{
public:
	executor();

	executor(const char* name);

	executor(const string& name);

	executor(const string_view name);

	~executor();

	template <class _Func, class... _Arg>
	const object_ptr operator()(_Func& func,  _Arg... arg) const;

protected:
	bool load_module(const object_ptr name) noexcept;

	bool load_func(const object_ptr func) const noexcept;
private:
	object_ptr m_module{ nullptr };
}; // class executor

} // namespace ss::lib::python

using namespace ss::lib::python;

template <class T>
size_t get_size(T& t)
{
	return tuple_size<T>{};
}

template <class _Func, class... _Arg>
const object_ptr executor::operator()(_Func& func,  _Arg... arg) const
{
	if (object_ptr _func = decoder(m_module, func)(); _func != nullptr && PyCallable_Check(_func) != 0)
	{
		auto _t = std::make_tuple(std::forward<_Arg>(arg)...);
	}
	return nullptr;
}


#endif // _PYTHON_IMPL_HPP_
