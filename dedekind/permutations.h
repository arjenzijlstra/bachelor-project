
#ifndef PERMUTATIONS_H_
#define PERMUTATIONS_H_

#include <algorithm>
#include <array>
#include <bitset>
#include <set>
#include <vector>

#include "powersetbin.h"


namespace Dedekind
{

namespace Internal
{


template <size_t size>
std::bitset<size> permute(std::array<size_t, size> const &permutation,
		std::bitset<size> const &elem)
{
	std::bitset<size> result;
	for (size_t idx = 0; idx != result.size(); ++idx)
	{
		result[idx] = elem[permutation[idx]];
	}
	return result;
}

template <size_t Number, size_t Power>
std::array<size_t, Power> subsetPermutation(
		std::array<size_t, Number> const &permutation,
		std::vector<std::bitset<Number>> const &pset)
{
	std::array<size_t, Power> result;
	size_t idx = 0;
	for (auto iter = pset.begin(); iter != pset.end(); ++iter)
	{
		std::bitset<Number> tmp = permute(permutation, *iter);
		result[idx++] = find(pset.begin(), pset.end(), tmp) - pset.begin();
	}
	return result;
}

template <size_t Number, size_t Power>
std::vector<std::array<size_t, Power>> permutations()
{
	std::vector<std::bitset<Number>> powerset =
			Internal::PowerSet<Number>::powerSetBin();

	std::array<size_t, Number> permutation;
	for (size_t idx = 0; idx != Number; ++idx)
	{
		permutation[idx] = idx;
	}

	std::vector<std::array<size_t, Power>> result;
	do
	{
		result.push_back(
				Internal::subsetPermutation<Number, Power>(permutation, powerset));
	}
	while (std::next_permutation(permutation.begin(), permutation.end()));

	return result;
}

template <size_t size>
std::set<std::bitset<size>, BitSetLess> equivalences(
		std::bitset<size> const &bset,
		std::vector<std::array<size_t, size>> const &perms)
{
	std::set<std::bitset<size>, BitSetLess> result;
	for (auto iter = perms.begin(); iter!= perms.end(); ++iter)
	{
		std::bitset<size> temp = permute(*iter, bset);
		result.insert(temp);
	}
	return result;
}


} // namespace Internal

} // namespace Dedekind


#endif
