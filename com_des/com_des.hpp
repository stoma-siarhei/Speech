#ifndef _COM_DES_HPP_
#define _COM_DES_HPP_


#include <pch.hpp>


using namespace std;

namespace ss::lib::cl
{

class deserialize
{
using set_t = unordered_set<string_view>;

using un_map_t = unordered_map<string_view, string_view>;

public:
	deserialize();

	deserialize(int argc, char** argv);

	~deserialize();

	bool operator()(int argc, char** argv);

	bool operator()(const string& str);

	string_view operator[](const string_view s) const;

protected:
	bool parse_command(const string_view str);

	bool parse_command_line(int argc, char ** argv);

	bool parse_command_line(const string_view str);

private:
	set_t m_set_command;
	un_map_t m_map_params;

	string m_command{ "-str -char -byte -short -int -long -float -double -bool -file -script -func" };
	string m_command_line;
}; // class deserialize

} // namespace ss::lib::cl


#endif // _COM_DES_HPP_
