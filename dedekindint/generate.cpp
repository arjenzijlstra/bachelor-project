
#include "dedekindint.ih"

Dedekind::setOfMonotoneSubsets Dedekind::generate(
		Dedekind::setOfMonotoneSubsets const &m1, size_t n)
{
	setOfMonotoneSubsets m2;
	size_t mn = 0;

	for (auto iter = m1.begin(); iter != m1.end(); ++iter)
	{
		for (auto iter2 = m1.begin(); iter2 != m1.end(); ++iter2)
		{
			if (*iter <= *iter2)
			{
				monotoneSubset tmp =
						Internal::concatenate(*iter, *iter2, n);
				m2.push_back(tmp);
				++mn;
			}
		}
	}

	return m2;
}
