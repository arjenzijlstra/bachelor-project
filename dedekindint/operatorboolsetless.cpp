
#include "dedekindint.ih"

#if 0

bool SetLess::operator()(set<size_t> const &lhs,
		set<size_t> const &rhs) const
{
	// if (lhs.size() != rhs.size())
	//     return lhs.size() < rhs.size();

	auto iter = lhs.begin(), iter2 = rhs.begin();
	for ( ; iter != lhs.end() && iter2 != rhs.end(); ++iter, ++iter2)
	{
		if ((*iter) != (*iter2))
		{
			return (*iter) < (*iter2);
		}
	}

	if (iter2 == rhs.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

#endif 0
