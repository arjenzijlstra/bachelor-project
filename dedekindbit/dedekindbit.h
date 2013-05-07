
#ifndef DEDEKINDBIT_H_
#define DEDEKINDBIT_H_

#include <iostream>
#include <set>
#include <bitset>
#include <vector>
#include <parallel/algorithm>
#include <algorithm>
#include <omp.h>
#include "../dedekindbase/dedekindbase.h"

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
/*
std::set<std::bitset<size>> powerset(std::set<std::bitset<size>> base)
{

}*/

template <size_t size>
std::bitset<size> reverse(std::bitset<size> bset)
{
	std::bitset<size> reverse;
	for (size_t iter = 0; iter != size; ++iter)
	{
		reverse[iter] = bset[size - iter - 1];
	}
	return reverse;
}

template <size_t size>
std::bitset<size> dual(std::bitset<size> bset)
{
	return reverse(bset).flip();
}

class DedekindBit // : public DedekindBase<std::bitset<1>, BitSetLess>
{

	public:
		DedekindBit() {};

		void generateMonotoneSubsets(size_t n)
		{
			std::vector<std::bitset<1>> result0({std::bitset<1>(0),
					std::bitset<1>(1)});
			std::vector<std::bitset<2>> result1(generate(result0));
			std::vector<std::bitset<4>> result2(generate(result1));
			std::vector<std::bitset<8>> result3(generate(result2));
			std::vector<std::bitset<16>> result4(generate(result3));
			std::vector<std::bitset<32>> result5(generate(result4));
			std::vector<std::bitset<64>> result6(generate(result5));

			std::cout << result0.size() << '\n';
			std::cout << result1.size() << '\n';
			std::cout << result2.size() << '\n';
			std::cout << result3.size() << '\n';
			std::cout << result4.size() << '\n';
			std::cout << result5.size() << '\n';
			std::cout << result6.size() << '\n';

			std::cout << generate(result6);
		}


	// Make it possible to accept sets of bitsets using different Compare
	// and Allocator classes
	template<size_t size>
	std::vector<std::bitset<(size << 1)>> generate(
			std::vector<std::bitset<size>> const &m1, size_t n = 0)
	{
		std::vector<std::bitset<(size << 1)>> m2;
		size_t mn = 0;

		for_each(m1.begin(), m1.end(),
		[&](std::bitset<size> const &iter)
		{
			for_each(m1.begin(), m1.end(),
			[&](std::bitset<size> const &iter2)
			{
				if (iter <= iter2)
				{
					m2.push_back(concatenate(iter, iter2));
					++mn;
				}
			});
		});

		return m2;
	}

	size_t generate(std::vector<std::bitset<64>> const &m1)
	{
		size_t mn = 0;

		#pragma omp parallel for reduction(+:mn) shared(m1) schedule(static, 1)
		for (size_t idx1 = 0; idx1 < m1.size(); ++idx1)
		{
			for (size_t idx2 = 0; idx2 < m1.size(); ++idx2)
			{
				if (m1[idx1] <= m1[idx2])
				{
					++mn;
				}
			}
			//if (omp_get_thread_num() == 0)
			//{
				if (idx1 % 1000 == omp_get_thread_num())
				{
					std::cout << idx1 << ": " << mn << '\n';
				}
			//}
		}
		return mn;
	}

	template <size_t size>
	std::bitset<(size << 1)> concatenate(std::bitset<size> const &lhs,
			std::bitset<size> const &rhs, size_t n = 0)
	{
		size_t const new_size = (size << 1);
		std::bitset<new_size> tmp_lhs(lhs.to_ullong() << size);
		std::bitset<new_size> tmp_rhs(rhs.to_ullong());

		return (tmp_lhs | tmp_rhs);
	}
};


#endif // end of guard DEDEKINDBIT_H_
