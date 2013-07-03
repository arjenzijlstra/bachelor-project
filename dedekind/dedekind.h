
#ifndef DEDEKIND_H_
#define DEDEKIND_H_

#include <algorithm>
#include <bitset>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "../uint128/uint128.h"

#include "bitsetless.h"
#include "bitsetoperleq.h"
#include "operwiedemann.h"
#include "permutations.h"
#include "powerof2.h"
#include "powersetbin.h"
#include "vectoroperinsert.h"

namespace Dedekind
{
	enum
	{
		BIGINTTAG
	};

	template <size_t Number, size_t Power>
	std::vector<std::vector<std::bitset<Power>>> generateRn(
			std::vector<std::bitset<Power>> const &dn)
	{
		auto permutations =
				Internal::permutations<Number, Power>();


		std::vector<std::vector<std::bitset<Power>>> rn;
		std::set<std::bitset<Power>, BitSetLess> processed;
		for (auto iter = dn.begin(); iter != dn.end(); ++iter)
		{
			if (processed.find(*iter) == processed.end())
			{
				auto equivs = Internal::equivalences(*iter, permutations);

				std::vector<std::bitset<Power>> permuted;
				copy(equivs.begin(), equivs.end(), std::back_inserter(permuted));
				for (auto perm = equivs.begin(); perm != equivs.end(); ++perm)
				{
					processed.insert(*perm);
				}

				rn.push_back(permuted);
			}
		}

		return rn;
	}

	template <size_t size>
	UInt128 enumerate(std::vector<std::bitset<size>> const &dn,
			std::vector<std::vector<std::bitset<size>>> const &rn,
			size_t rank = 0, size_t nprocs = 1)
	{
		std::map<std::bitset<size>, std::bitset<size>, BitSetLess> duals;
		std::map<std::bitset<size>, size_t, BitSetLess> etas;

		// Preprocess duals and eta's of all elements
		for (auto iter = rn.begin(); iter != rn.end(); ++iter)
		{
			auto elem = (*iter).begin();
			size_t tmp = Internal::eta(*elem, dn);
			for (; elem != (*iter).end(); ++elem)
			{
				etas[*elem] = tmp;
				duals[*elem] = Internal::dual(*elem);
			}
		}
		// Preprocessing complete


		UInt128 result;
		for (size_t idx = rank; idx < rn.size(); idx += nprocs)
		{
			auto iter(rn[idx].begin());
			for (auto iter2 = dn.begin(); iter2 != dn.end(); ++iter2)
			{
				auto first = *iter & *iter2;
				auto second = duals[*iter] & duals[*iter2];

				result += rn[idx].size() * etas[first] * etas[second];
			}
		}

		return result;
	}

	template <size_t size>
	std::vector<std::bitset<(size << 1)>> generate(
			std::vector<std::bitset<size>> const &dn)
	{
		std::vector<std::bitset<(size << 1)>> dn1;

		for (auto iter = dn.begin(); iter != dn.end(); ++iter)
		{
			for (auto iter2 = dn.begin(); iter2 != dn.end(); ++iter2)
			{
				if (*iter <= *iter2)
				{
					dn1.push_back(Internal::concatenate(*iter, *iter2));
				}
			}
		}

		return dn1;
	}

	namespace Internal
	{
		template <size_t Number>
		struct MonotoneSubsets
		{
			static std::vector<std::bitset<PowerOf2<Number>::value>> result;
		};

		template <size_t Number>
		std::vector<std::bitset<PowerOf2<Number>::value>>
				MonotoneSubsets<Number>::result(generate(
						MonotoneSubsets<(Number - 1)>::result));

		template <>
		struct MonotoneSubsets<0>
		{
			static std::vector<std::bitset<1>> result;
		};

		std::vector<std::bitset<1>> MonotoneSubsets<0>::result({std::bitset<1>(0),
				std::bitset<1>(1)});
	}

	template <size_t Number>
	UInt128 monotoneSubsets(size_t rank = 0, size_t size = 1)
	{
		auto dn = Internal::MonotoneSubsets<Number - 2>::result;

		std::cerr << "Rank " << rank << " is done generating D"
				<< Number - 2 << ": " << dn.size() <<  '\n';

		auto rn = generateRn<Number - 2>(dn);

		std::cerr << "Rank " << rank << " is done generating R"
				<< Number - 2 << ": " << rn.size() << '\n';

    	UInt128 result = enumerate(dn, rn, rank, size);

    	return result;
	}
}


#endif // end of guard DEDEKIND_H_
