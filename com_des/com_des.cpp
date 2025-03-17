#include "com_des.hpp"


using namespace std;

namespace ss::lib::cl
{

deserialize::deserialize()
{
	parse_command(m_command);
}

ss::lib::cl::deserialize::deserialize(int argc, char** argv)
	: deserialize()
{
	parse_command_line(argc, argv);
}

deserialize::~deserialize()
{
}

bool deserialize::operator()(int argc, char** argv)
{
	return parse_command_line(argc, argv);
}

bool deserialize::operator()(const string& str)
{
	return parse_command_line(str);
}

string_view deserialize::operator[](const string_view s) const
{
	if (auto it = m_map_params.find(s); it == end(m_map_params))
	{
		string str{ "No find element - \"" };
		str += s;
		str += "\"";
		throw out_of_range(str);
	}
	else return m_map_params.at(s);
}

bool ss::lib::cl::deserialize::parse_command(const string_view str)
{
	if (str.empty()) return true;

	if (auto _pos = str.find(' '); _pos == string_view::npos) m_set_command.insert(str);
	else
	{
		m_set_command.insert(str.substr(0, _pos));
		return parse_command(str.substr(++_pos));
	}

	return true;
}

bool ss::lib::cl::deserialize::parse_command_line(int argc, char** argv)
{
	for (int it{ 1 }; it < argc; it++)
	{
		m_command_line += ' ';
		m_command_line += argv[it];
	}

	return parse_command_line(m_command_line);
}

bool deserialize::parse_command_line(const string_view str)
{
	if (str.empty()) return false;

	un_map_t m;
	string_view s;
	vector<size_t> v;
	for (size_t min{ string_view::npos }; auto && it : m_set_command)
	{
		if (auto p = str.find(it); p != string_view::npos)
		{
			v.push_back(p);
			if (min > p)
			{
				min = p;
				s = it;
			}
		}
	}
	
	if (s.empty()) return false;

	sort(begin(v), end(v));
	if (v.size() > 1) 
		m_map_params.insert({ s, str.substr(v[0] + s.size() + 1, v[1] - s.size() - v[0] - 2)});
	else
	{
		m_map_params.insert({ s, str.substr(v[0] + s.size() + 1) });
		return true;
	}
	
	return parse_command_line(str.substr(v[1]));
}

} // namespace ss::lib::cl
