
#include "dedekindint.ih"

ostream &operator<<(ostream &out,
	set<set<set<size_t>, SetLess>, SetSetLess> const &rhs)
{
	for (auto iter = rhs.begin(); iter != rhs.end(); ++iter)
	{
		if ((*iter).size() == 0)
		{
			out << "E\n";
		}
		else
		{
			out << *iter;
		}
	}
	return out;
}
