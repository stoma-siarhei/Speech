#ifndef _PYTHON_IMPL_HPP_
#define _PYTHON_IMPL_HPP_


#include <pch.hpp>

#include "python_types.hpp"
#include "utils.hpp"


using namespace std;

namespace ss::lib::python
{

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

struct variable
{
	variable();

	~variable();

	template <class _Tuple, size_t N = tuple_size_v<_Tuple>>
	object_ptr operator()(const _Tuple& _tuple);

protected:
	template <class _Tuple, size_t... In>
	void tuple_index(const _Tuple& _tuple, index_sequence<In...>);

	template <class _Type>
	void convert(const _Type& var, const size_t index);

private:
	object_ptr m_variable;
}; // struct variable

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

template <class _Tuple, size_t N>
object_ptr variable::operator()(const _Tuple& _tuple)
{
	m_variable = PyTuple_New(tuple_size_v<_Tuple>);

	tuple_index(_tuple, make_index_sequence<N>{});

	return m_variable;
}

template <class _Tuple, size_t... In>
void variable::tuple_index(const _Tuple& _tuple, index_sequence<In...>)
{
	(convert(get<In>(_tuple), In), ...);
}

template <class _Type>
void variable::convert(const _Type& var, const size_t index)
{
	object_ptr _py_obj;

	if constexpr ((is_same_v<_Type, string>) || (is_same_v<_Type, const string>))
	{
		_py_obj = PyUnicode_FromString(var.data());
	}
	else if constexpr ((is_same_v<_Type, char*>) || (is_same_v<_Type, const char*>))
	{
		_py_obj = PyUnicode_FromString(var);
	}
	else if constexpr (is_integral_v<_Type>)
	{
		_py_obj = PyLong_FromLong(var);
	}
	else if constexpr (is_floating_point_v<_Type>)
	{
		_py_obj = PyFloat_FromDouble(var);
	}
	else
	{
		return;
	}

	PyTuple_SetItem(m_variable, index, _py_obj);
}

template <class _Func, class... _Arg>
const object_ptr executor::operator()(_Func& func,  _Arg... arg) const
{
	if (object_ptr _func = decoder(m_module, func)(); _func != nullptr && PyCallable_Check(_func) != 0)
	{
		auto _t = std::make_tuple(std::forward<_Arg>(arg)...);
		variable v;
		auto arg = v(_t);
	}
	return nullptr;
}


#endif // _PYTHON_IMPL_HPP_
