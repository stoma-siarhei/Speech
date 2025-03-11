#ifndef _PYTHON_TYPES_HPP_
#define _PYTHON_TYPES_HPP_


#include <include/Python.h>


namespace ss::lib::python
{

using object_t = PyObject;

using object_ptr = PyObject*;

} // namespace ss::lib::python


#endif // _PYTHON_TYPES_HPP_
