
#ifndef VECTOROPERINSET_H_
#define VECTOROPERINSET_H_

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "bitsetless.h"
#include "powerof2.h"
#include "powersetbin.h"

template <typename Type>
std::ostream &operator<<(std::ostream &out,
	std::vector<Type> const &rhs)
{
	out << '{';
	for (auto iter = rhs.begin();
			iter != rhs.end();
				++iter)
	{
		out << ((iter != rhs.begin()) ? ", " : "") << *iter;
	}
	out << '}';

	return out;
}

template <size_t size>
std::string bitsetToString(std::bitset<size> const &bset,
		std::array<std::string, size> const &domain, std::string const &sep)
{
	std::string result;
	for (size_t idx = 0; idx != bset.size(); ++idx)
	{
		if (bset[idx])
		{
			std::stringstream ss(domain[idx]);
			result.append(ss.str() + sep);;
		}
	}
	if (bset.none())
	{
		result.append("e");
	}
	return result;
}

template <size_t size>
std::string powersetToString(std::vector<std::bitset<size>> const &pset)
{
	std::array<std::string, size> domain;
	for (size_t idx = 0; idx != size; ++idx)
	{
		std::stringstream ss;
		ss << idx;
		domain[idx] = ss.str();
	}

	std::string result;
	for (auto iter = pset.begin(); iter != pset.end(); ++iter)
	{
		result.append(bitsetToString((*iter), domain, "") + ", ");
	}
	return result;
}

template <size_t size>
std::string subsetToString(std::vector<std::bitset<size>> const &sset)
{
	size_t const logof2(Dedekind::Internal::LogOf2<size>::value);
	std::vector<std::bitset<logof2>> powerset =
			Dedekind::Internal::PowerSet<logof2>::powerSetBin();


	std::sort(powerset.begin(), powerset.end(), Dedekind::bitsetLess<logof2>);

	std::array<std::string, logof2> domain1;
	for (size_t idx = 0; idx != logof2; ++idx)
	{
		std::stringstream ss;
		ss << idx;
		domain1[idx] = ss.str();
	}

	std::cout << domain1.size() << " " << size << " " << powerset.size() << '\n';

	std::array<std::string, size> domain2;
	for (size_t idx = 0; idx != powerset.size(); ++idx)
	{
		domain2[idx] = bitsetToString(powerset[idx], domain1, "");
	}

	std::string result;
	for (auto iter = sset.begin(); iter != sset.end(); ++iter)
	{
		result.append(bitsetToString((*iter), domain2, " ") + "\n");
	}
	return result;
}

#endif
