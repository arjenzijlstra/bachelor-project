
#ifndef DEDEKIND_H_
#define DEDEKIND_H_

#include <iostream>
#include <unordered_map>
#include <bitset>
#include <vector>
#include <algorithm>
#include <omp.h>

//#include <gmpxx.h> // for big numbers

#include "../uint128/uint128.h"

#define PREPROCESSING 1
#define PRAGMAOMP 0
#define DYNAMIC 0
#define ONTHESPOT 0

class BitSetLess
{
	public:
		template<size_t size>
		bool operator()(std::bitset<size> const &lhs,
				std::bitset<size> const &rhs) const
		{
			return lhs.to_ulong() > rhs.to_ulong();
		}
};

template <typename Type>
std::ostream &operator<<(std::ostream &out,
	std::vector<Type> const &rhs)
{
	out << '{';
	for (auto iter = rhs.begin();
			iter != rhs.end();
				++iter)
	{
		out << ((iter != rhs.begin()) ? ",\n" : "") << *iter;
	}
	out << '}';

	return out;
}

template <size_t size>
bool operator<=(std::bitset<size> lhs, std::bitset<size> const &rhs)
{
	return (lhs.flip() | rhs).all();

	// I guess the above statement is faster.
	// for (size_t idx = 0; idx != size; ++idx)
	// {
	//     if (lhs[idx] && !rhs[idx])
	//     {
	//         return false;
	//     }
	// }
	// return true;
}

namespace Dedekind
{
	enum
	{
		BIGINTTAG
	};

	namespace Internal
	{
		bool powersetLess(std::set<size_t> const &lhs,
				std::set<size_t> const &rhs)
		{
			if (lhs.size() != 0 && rhs.size() != 0)
			{
				auto iter2 = rhs.rbegin();
				for (auto iter = lhs.rbegin(); iter != lhs.rend(); ++iter)
				{
					if (iter2 == rhs.rend())
					{
						break;
					}
					if (*iter != *iter2)
					{
						return *iter < *iter2;
					}
					++iter2;
				}
				return lhs.size() < rhs.size();
			}
		/*
			if (lhs.size() == rhs.size())
			{
				for (auto iter = lhs.begin(), iter2 = rhs.begin(); iter != lhs.end();
						++iter, ++iter2)
				{
					if (*iter != *iter2)
					{
						return *iter < *iter2;
					}
				}
			}
		//*/
			return lhs.size() < rhs.size();
		}

		void powersetRec(std::vector<std::set<size_t>> &result,
							  std::set<size_t> current, size_t const n)
		{
			if (n == 0)
			{
				result.push_back(current);
				return;
			}

			size_t newN(n - 1);
			powersetRec(result, current, newN);
			current.insert(newN);
			powersetRec(result, current, newN);
		}

		template <size_t size>
		std::array<std::set<size_t>, size> powerset(size_t const n)
		{
			std::vector<std::set<size_t>> tmpResult;
			std::set<size_t> current;
			powersetRec(tmpResult, current, n);

			// sort the powerset as needed and put it into an std::array
			std::sort(tmpResult.begin(), tmpResult.end(), powersetLess);
			std::array<std::set<size_t>, size> result;
			std::copy(tmpResult.begin(), tmpResult.end(), result.begin());

			return result;
		}

		template <size_t size>
		std::set<size_t> permute(std::array<size_t, size> const &permutation,
				std::set<size_t> const &elem)
		{
			std::set<size_t> result;
			for (auto iter = elem.begin(); iter != elem.end(); ++iter)
			{
				result.insert(permutation[*iter]);
			}
			return result;
		}

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

		template <size_t size, size_t size2>
		std::array<size_t, size2> mbfPermutation(
				std::array<size_t, size> const &permutation,
				std::array<std::set<size_t>, size2> const &ps)
		{
			std::array<size_t, size2> result;
			size_t idx = 0;
			for (auto iter = ps.begin(); iter != ps.end(); ++iter)
			{
				std::set<size_t> tmp = permute(permutation, *iter);
				result[idx++] = find(ps.begin(), ps.end(), tmp) - ps.begin();
			}
			return result;
		}

		template <size_t size>
		std::set<std::bitset<size>, BitSetLess> permutations(
				std::bitset<size> const &bs,
				std::vector<std::array<size_t, size>> const &perms)
		{
			std::set<std::bitset<size>, BitSetLess> result;
			for (auto iter = perms.begin(); iter!= perms.end(); ++iter)
			{
				std::bitset<size> temp = permute(*iter, bs);
				result.insert(temp);
			}
			return result;
		}

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
			size_t const new_size = (size << 1);
			std::bitset<new_size> tmp_lhs(lhs.to_ullong() << size);
			std::bitset<new_size> tmp_rhs(rhs.to_ullong());

			return (tmp_lhs | tmp_rhs);
		}
	} // end namespace Internal


	template <size_t Number, size_t Power>
	std::vector<std::vector<std::bitset<Power>>> generateRn(
			std::vector<std::bitset<Power>> const &dn)
	{
		std::array<size_t, Number> permutation;

		for (size_t idx = 0; idx != Number; ++idx)
		{
			permutation[idx] = idx;
		}

		std::array<std::set<size_t>, Power> powerSet =
				Internal::powerset<Power>(Number);

		std::vector<std::array<size_t, Power>> mbfPermutations;
		do
		{
			mbfPermutations.push_back(
					Internal::mbfPermutation(permutation, powerSet));
		}
		while (std::next_permutation(permutation.begin(), permutation.end()));

		std::vector<std::vector<std::bitset<Power>>> rn;
		std::set<std::bitset<Power>, BitSetLess> processed;

		for (auto iter = dn.begin(); iter != dn.end(); ++iter)
		{
			if (processed.find(*iter) == processed.end())
			{
				auto permuted = Internal::permutations(*iter, mbfPermutations);
				std::vector<std::bitset<Power>> vecPermuted;
				copy(permuted.begin(), permuted.end(), std::back_inserter(vecPermuted));
				for (auto perm = permuted.begin(); perm != permuted.end(); ++perm)
				{
					processed.insert(*perm);
				}

				rn.push_back(vecPermuted);
			}
		}

		for (auto iter = rn.begin(); iter != rn.end(); ++iter)
		{
			//std::sort((*iter).begin(), (*iter).end(), BitSetLess);
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

		std::cerr << rank << ": Preprocessing complete.\n";

		UInt128 result(0);
		// #pragma omp parallel for reduction(+:result) shared(dn) schedule(static, 1)
		for (size_t idx = rank; idx < rn.size(); idx += nprocs)
		{
			size_t counter = 0;
			auto iter(rn[idx].begin());
			//std::cout << idx << '\n';
			for (auto iter2 = dn.begin(); iter2 != dn.end(); ++iter2)
			{
#if 1
				auto first = *iter & *iter2;
				auto second = duals[*iter] & duals[*iter2];

				result += rn[idx].size() * etas[first] * etas[second];

				if (counter % 10000 == 0)
				{
					//std::cerr << counter << " " << result << '\n';
				}
				++counter;
#else
				result += rn[idx].size() * Internal::eta(*iter & *iter2, dn)
						 * Internal::eta(Internal::dual(*iter) & Internal::dual(*iter2), dn);
#endif
#if 0
				result += etas[iter & *iter2]
						  * etas[duals[iter] & duals[*iter2]];
#endif
			}
		}

		return result;
	}

	template <size_t size>
	std::vector<std::bitset<(size << 1)>> generate(
			std::vector<std::bitset<size>> const &m1)
	{
		std::vector<std::bitset<(size << 1)>> m2;
		size_t mn = 0;

		for (auto iter = m1.begin(); iter != m1.end(); ++iter)
		{
			for (auto iter2 = m1.begin(); iter2 != m1.end(); ++iter2)
			{
				if (*iter <= *iter2)
				{
					m2.push_back(Internal::concatenate(*iter, *iter2));
					++mn;
				}
			}
		}

		return m2;
	}

	namespace Internal
	{
		template<int P>
		struct PowerOf2
		{
			static size_t const value;
		};

		template<int P>
		size_t const PowerOf2<P>::value(2 * PowerOf2<P - 1>::value);

		template<>
		struct PowerOf2<0>
		{
			static size_t const value;
		};

		size_t const PowerOf2<0>::value(1);


		template<size_t Number>
		struct MonotoneSubsets
		{
			static std::vector<std::bitset<PowerOf2<Number>::value>> result;
		};

		template<size_t Number>
		std::vector<std::bitset<PowerOf2<Number>::value>>
				MonotoneSubsets<Number>::result(generate(
						MonotoneSubsets<(Number - 1)>::result));

		template<>
		struct MonotoneSubsets<0>
		{
			static std::vector<std::bitset<1>> result;
		};

		std::vector<std::bitset<1>> MonotoneSubsets<0>::result({std::bitset<1>(0),
				std::bitset<1>(1)});
	}

	template<size_t Number>
	UInt128 monotoneSubsets(size_t rank = 0, size_t size = 1)
	{
		// no need to do this if (rank == 0) and then Bcast it because the other
		// threads will just wait for it anyway
		auto dn = Internal::MonotoneSubsets<Number - 2>::result;
		// MPI::Comm::Bcast(&tmp, .. )

		if (rank == 0)
		{
			std::cerr << "Done generating D. Total size: " << dn.size() << '\n';
		}

		auto rn = generateRn<Number - 2>(dn);

		if (rank == 0)
		{
			std::cerr << "Done generating R. Total size: " << rn.size() << '\n';
		}

    	UInt128 result = enumerate(dn, rn, rank, size);

    	return result;
	}
}


#endif // end of guard DEDEKIND_H_
