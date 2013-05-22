
#ifndef DEDEKIND_H_
#define DEDEKIND_H_

#include <iostream>
#include <unordered_map>
#include <bitset>
#include <vector>
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
	enum
	{
		BIGINTTAG
	};

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
	mpz_class enumerate(std::vector<std::bitset<size>> const &dn,
			size_t rank = 0, size_t nprocs = 1)
	{
		mpz_class result = 0;

		std::unordered_map<std::bitset<size>, std::bitset<size>> duals;
		std::unordered_map<std::bitset<size>, size_t> etas;

//*		// Preprocess duals and eta's of all elements
		for (size_t idx = 0; idx < dn.size(); ++idx)
		{
			duals[dn[idx]] = Internal::dual(dn[idx]);
			etas[dn[idx]] = Internal::eta(dn[idx], dn);
		}
//*/
		std::cerr << "Preprocessing complete\n";

		// #pragma omp parallel for reduction(+:result) shared(dn) schedule(static, 1)
		for (size_t idx1 = rank; idx1 < dn.size(); idx1 += nprocs)
		{
			for (auto iter2 = dn.begin(); iter2 != dn.end(); ++iter2)
			{
				auto iter(dn[idx1]);
/*
				auto tmp = iter & *iter2;
				size_t first;
				if (etas.find(tmp) == etas.end())
				{
					first = Internal::eta(tmp, dn);
					etas[tmp] = first;
				}
				else
				{
					first = etas[tmp];
				}

				std::bitset<size> dual1;
				if (duals.find(iter) == duals.end())
				{
					dual1 = Internal::dual(iter);
					duals[iter] = dual1;
				}
				else
				{
					dual1 = duals[iter];
				}

				std::bitset<size> dual2;
				if (duals.find(*iter2) == duals.end())
				{
					dual2 = Internal::dual(*iter2);
					duals[*iter2] = dual2;
				}
				else
				{
					dual2 = duals[*iter2];
				}

				auto tmp2 = dual1 & dual2;
				size_t second;
				if (etas.find(tmp2) == etas.end())
				{
					second = Internal::eta(tmp2, dn);
					etas[tmp2] = second;
				}
				else
				{
					second = etas[tmp2];
				}
				result += first * second;
//*/


				// result += Internal::eta(iter & *iter2, dn)
				// 		 * Internal::eta(Internal::dual(iter) & Internal::dual(*iter2), dn);
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
	mpz_class monotoneSubsets(size_t rank = 0, size_t size = 1)
	{
		// no need to do this if (rank == 0) and then Bcast it because the other
		// threads will not do anything anyway
		auto tmp = Internal::MonotoneSubsets<Number - 2>::result;
		// MPI::Comm::Bcast(&tmp, .. )

    	mpz_class result = enumerate(tmp, rank, size);

    	return result;
	}
}


#endif // end of guard DEDEKIND_H_
