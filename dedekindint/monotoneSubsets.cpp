
#include "dedekindint.ih"

Dedekind::setOfMonotoneSubsets Dedekind::monotoneSubsets(size_t n)
{
	Dedekind::setOfMonotoneSubsets result({{ }, {{ }}});
	for (size_t iter = 0; iter != n; ++iter)
	{
		result = generate(result, iter);
	}
	return result;
}
