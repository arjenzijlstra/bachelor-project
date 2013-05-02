
#include "dedekindint.ih"

bool SetSetLess::operator()(set<set<size_t>, SetLess> const &lhs,
		set<set<size_t>, SetLess> const &rhs) const
{
	if (lhs.size() != rhs.size())
		return lhs.size() < rhs.size();

	auto iter2 = rhs.begin();
	for (auto iter = lhs.begin(); iter != lhs.end(); ++iter)
	{
		if ((*iter).size() != (*iter2).size())
		{
			return (*iter).size() < (*iter2).size();
		}

		auto iter4 = (*iter2).begin();
		for (auto iter3 = (*iter).begin(); iter3 != (*iter).end(); ++iter3)
		{
			if (*iter3 != *iter4)
			{
				return *iter3 < *iter4;
			}

			++iter4;
		}

		++iter2;
	}
	return true;
}
