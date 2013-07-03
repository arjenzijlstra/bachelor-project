
#ifndef OPERWIEDEMANN_H_
#define OPERWIEDEMANN_H_

#include <bitset>
#include <vector>
#include <string>

#include "bitsetoperleq.h"


namespace Dedekind
{

namespace Internal
{


template <size_t size>
std::bitset<size> reverse(std::bitset<size> const &bset)
{
	std::bitset<size> reverse;
	for (size_t iter = 0; iter != size; ++iter)
	{
		reverse[iter] = bset[size - iter - 1];
	}
	return reverse;
}

template <size_t size>
std::bitset<size> dual(std::bitset<size> const &bset)
{
	return reverse(bset).flip();
}

template <size_t size>
size_t eta(std::bitset<size> const &bset,
			  std::vector<std::bitset<size>> const &dn)
{
	size_t result = 0;
	for (size_t idx = 0; idx < dn.size(); ++idx)
	{
		if (dn[idx] <= bset)
		{
			++result;
		}
	}
	return result;
}

template <size_t size>
std::bitset<(size << 1)> concatenate(std::bitset<size> const &lhs,
		std::bitset<size> const &rhs)
{
	std::string lhs_str = lhs.to_string();
	std::string rhs_str = rhs.to_string();

	return std::bitset<(size << 1)>(lhs_str + rhs_str);
}


} // namespace Internal

} // namespace Dedekind


#endif
