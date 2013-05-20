
#ifndef DEDEKINDINT_H_
#define DEDEKINDINT_H_

#include <set>
#include <algorithm>
#include <iostream>

#if 0  // chose for standard or own implementation

#define SetLess std::less<std::set<size_t>>

#else

class SetLess
{
	public:
		bool operator()(std::set<size_t> const &lhs,
				std::set<size_t> const &rhs) const;
};

#endif

#if 0  // chose for standard or own implementation

#define SetSetLess std::less<std::set<std::set<size_t>, SetLess>>

#else

class SetSetLess
{
	public:
		bool operator()(std::set<std::set<size_t>, SetLess> const &lhs,
				std::set<std::set<size_t>, SetLess> const &rhs) const;
};

#endif

template <typename Type>
std::vector<Type> operator+(std::vector<Type> const &lhs,
	std::vector<Type> const &rhs)
{
	std::vector<Type> result;
	std::set_union(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
		std::inserter(result, result.end()));

	return result;
}


template <typename Type, typename Less>
bool operator<=(std::set<Type, Less> const &lhs,
	std::set<Type, Less> const &rhs)
{
	return std::includes(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}


namespace Dedekind
{
	typedef std::vector<std::set<size_t>> monotoneSubset;
	typedef std::vector<monotoneSubset> setOfMonotoneSubsets;

	setOfMonotoneSubsets monotoneSubsets(size_t n);

	setOfMonotoneSubsets generate(setOfMonotoneSubsets const &m1, size_t n);

	namespace Internal
	{
		monotoneSubset concatenate(monotoneSubset const &lhs,
				monotoneSubset const &rhs, size_t n);
	}
}

std::ostream &operator<<(std::ostream &out, std::set<size_t> const &rhs);

std::ostream &operator<<(std::ostream &out,
		Dedekind::monotoneSubset const &rhs);

std::ostream &operator<<(std::ostream &out,
		Dedekind::setOfMonotoneSubsets const &rhs);


#endif // end of guard DEDEKINDINT_H_
