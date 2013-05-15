
#ifndef DEDEKIND_H_
#define DEDEKIND_H_

#include <iostream>
#include <unordered_map>
#include <bitset>
#include <vector>
#include <parallel/algorithm>
#include <algorithm>
#include <omp.h>

#include <gmpxx.h> // for big numbers

class BitSetLess
{
	public:
		template<size_t size>
		bool operator()(std::bitset<size> const &lhs,
				std::bitset<size> const &rhs) const
		{
			return lhs.to_ulong() < rhs.to_ulong();
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
		out << ((iter != rhs.begin()) ? ", " : "") << *iter;
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
			size_t const new_size = (size << 1);
			std::bitset<new_size> tmp_lhs(lhs.to_ullong() << size);
			std::bitset<new_size> tmp_rhs(rhs.to_ullong());

			return (tmp_lhs | tmp_rhs);
		}
	}

	template <size_t size>
	size_t enumerate(std::vector<std::bitset<size>> const &dn)
	{
		size_t result = 0;

		std::unordered_map<std::bitset<size>, std::bitset<size>> duals;
		std::unordered_map<std::bitset<size>, size_t> etas;

		// Preprocess duals and eta's of all elements
		for (size_t idx = 0; idx < dn.size(); ++idx)
		{
			duals[dn[idx]] = Internal::dual(dn[idx]);
			etas[dn[idx]] = Internal::eta(dn[idx], dn);
		}

		std::cerr << "Preprocessing complete\n";

		#pragma omp parallel for reduction(+:result) shared(dn) schedule(static, 1)
		for (size_t idx1 = 0; idx1 < dn.size(); ++idx1)
		{
			for (auto iter2 = dn.begin(); iter2 != dn.end(); ++iter2)
			{
				auto iter = dn[idx1];
				result += etas[iter & *iter2]
						* etas[duals[iter] & duals[*iter2]];
			}
		}
		return result;
	}

	template <size_t size>
	static std::vector<std::bitset<(size << 1)>> generate(
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
	size_t monotoneSubsets()
	{
		return enumerate(Internal::MonotoneSubsets<Number - 2>::result);
	}
}


#endif // end of guard DEDEKIND_H_
