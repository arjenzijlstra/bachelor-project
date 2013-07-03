
#include "main.ih"

typedef Dedekind::UInt128 (*fptr)(size_t, size_t);

template <size_t a = 8>
fptr findFunction(size_t b)
{
	if (a == b)
	{
		return Dedekind::monotoneSubsets<a>;
	}
	else
	{
		return findFunction<a - 1>(b);
	}
}

template <>
fptr findFunction<3>(size_t b)
{
	return Dedekind::monotoneSubsets<3>;
}
