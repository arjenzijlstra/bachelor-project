
#include "dedekindint.ih"

bool monotoneSubsetLess(set<size_t> const &lhs,
		set<size_t> const &rhs)
{
	if (lhs.size() == rhs.size())
	{
		for (auto iter = lhs.begin(), iter2 = rhs.begin(); iter != lhs.end();
				++iter, ++iter2)
		{
			if (*iter != *iter2)
			{
				return *iter < *iter2;
			}
		}
	}

	return lhs.size() < rhs.size();
}

ostream &operator<<(ostream &out,
		Dedekind::monotoneSubset const &rhs)
{
	vector<set<size_t>> tmp;
	copy(rhs.begin(), rhs.end(), back_inserter(tmp));
	sort(tmp.begin(), tmp.end(), monotoneSubsetLess);
	for (auto iter = tmp.begin(); iter != tmp.end(); ++iter)
	{
		if ((*iter).size() == 0)
		{
			out << "e ";
		}
		else
		{
			out << *iter;
		}
	}
	out << '\n';
	return out;
}
