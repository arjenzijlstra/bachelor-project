
#ifndef POWERSETBIN_H_
#define POWERSETBIN_H_

#include <vector>
#include <bitset>

namespace Dedekind
{

namespace Internal
{


template <size_t size>
struct PowerSet
{
	static std::vector<std::bitset<size>> powerSetBin();
};

template <size_t size>
std::vector<std::bitset<size>> PowerSet<size>::powerSetBin()
{
	auto current = PowerSet<size - 1>::powerSetBin();

	std::vector<std::bitset<size>> result;
	for (auto iter = current.begin(); iter != current.end(); ++iter)
	{
		std::bitset<size> tmp((*iter).to_ulong() + (1 << (size - 1)));
		result.push_back(tmp);
	}

	for (auto iter = current.begin(); iter != current.end(); ++iter)
	{
		std::bitset<size> tmp((*iter).to_ulong());
		result.push_back(tmp);
	}

	return result;
}


template <>
struct PowerSet<0>
{
	static std::vector<std::bitset<0>> powerSetBin();
};

std::vector<std::bitset<0>> PowerSet<0>::powerSetBin()
{
	return std::vector<std::bitset<0>>({ std::bitset<0>() });
}


} // namespace Internal

} // namespace Dedekind


#endif
