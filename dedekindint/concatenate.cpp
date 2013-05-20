
#include "dedekindint.ih"

Dedekind::monotoneSubset Dedekind::Internal::concatenate(
		Dedekind::monotoneSubset const &lhs,
		Dedekind::monotoneSubset const &rhs, size_t n)
{
	Dedekind::monotoneSubset result;
	for (auto iter = rhs.begin(); iter != rhs.end(); ++iter)
	{
		set<size_t> tmp(*iter);
		tmp.insert(n);
		result.push_back(tmp);
	}

	return (lhs + result);
}
