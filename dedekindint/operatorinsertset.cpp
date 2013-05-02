
#include "dedekindint.ih"

std::ostream &operator<<(std::ostream &out, std::set<size_t> const &rhs)
{
	for (auto iter = rhs.begin(); iter != rhs.end(); ++iter)
	{
		out << *iter;
	}
	out << ' ';
	return out;
}
