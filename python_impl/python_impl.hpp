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

	decoder(const object_ptr p_obj, const char* str);

	decoder(const object_ptr p_obj, const string& str);

	~decoder();

	shared_ptr<object_t> operator()() const noexcept;
private:
	object_ptr m_value{ nullptr };
}; // struct decoder

struct variable
{
	variable();

	~variable();

	template <class _Tuple, size_t N = tuple_size_v<_Tuple>>
	shared_ptr<object_t> operator()(const _Tuple& _tuple);

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
	using second = chrono::duration<double>;

	using millisecond = chrono::duration<double, ratio_multiply<second::period, milli>>;

	using microsecond = chrono::duration<double, ratio_multiply<second::period, micro>>;

	using result_exec_t = tuple<long, second>;

public:
	executor();

	executor(const char* name);

	executor(const string& name);

	~executor();

	template <class _Func, class... _Arg>
	const result_exec_t operator()(_Func& func,  _Arg... arg) const;

protected:
	bool load_module(const object_ptr name) noexcept;

	bool load_func(const object_ptr func) const noexcept;
private:
	object_ptr m_module{ nullptr };
}; // class executor

void deleter(object_ptr ptr);

} // namespace ss::lib::python

using namespace ss::lib::python;

template <class _Tuple, size_t N>
shared_ptr<object_t> variable::operator()(const _Tuple& _tuple)
{
	m_variable = PyTuple_New(tuple_size_v<_Tuple>);

	tuple_index(_tuple, make_index_sequence<N>{});

	return shared_ptr<object_t>(m_variable, deleter);
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
const executor::result_exec_t executor::operator()(_Func& func,  _Arg... arg) const
{
	const auto _start{ chrono::steady_clock::now()};

	if (auto _func = decoder(m_module, func)(); _func.get() != nullptr && PyCallable_Check(_func.get()) != 0)
	{
		auto _t = make_tuple(std::forward<_Arg>(arg)...);
		auto arg = variable()(_t);
		shared_ptr<object_t> res(PyObject_CallObject(_func.get(), arg.get()), deleter);
	
		const auto _stop{ chrono::steady_clock::now()};
		
		return { PyLong_AsLong(res.get()), _stop - _start };
	}
	string str = "Error decoder function script - ";
	str += func;
	throw runtime_error(str);
}


#endif // _PYTHON_IMPL_HPP_
