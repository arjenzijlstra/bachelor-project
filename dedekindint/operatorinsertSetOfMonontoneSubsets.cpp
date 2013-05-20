
#include "dedekindint.ih"

bool setOfMonotoneSubsetLess(Dedekind::monotoneSubset const &lhs,
		Dedekind::monotoneSubset const &rhs)
{
	if (lhs.size() == rhs.size())
	{
		for (auto iter = lhs.begin(), iter2 = rhs.begin(); iter != lhs.end();
				++iter, ++iter2)
		{
			if (*iter != *iter2)
			{
				return *iter > *iter2;
			}
		}
	}

	return lhs.size() > rhs.size();
}

ostream &operator<<(ostream &out,
		Dedekind::setOfMonotoneSubsets const &rhs)
{
	Dedekind::setOfMonotoneSubsets tmp(rhs);
	sort(tmp.begin(), tmp.end(), setOfMonotoneSubsetLess);
	for (auto iter = tmp.begin(); iter != tmp.end(); ++iter)
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
