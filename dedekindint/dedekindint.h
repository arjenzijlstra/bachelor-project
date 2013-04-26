
#ifndef DEDEKINDINT_H
#define DEDEKINDINT_H

#include <set>
#include <algorithm>
#include <iostream>

#include "../dedekindbase/dedekindbase.h"

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

template <typename Type, typename Less>
std::set<Type, Less> operator+(std::set<Type, Less> const &lhs,
    std::set<Type, Less> const &rhs)
{
    std::set<Type, Less> result;
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


class DedekindInt: public DedekindBase<
        std::set<std::set<size_t>, SetLess>, SetSetLess>
{
    typedef std::set<std::set<size_t>, SetLess> monotoneSubset;
    typedef std::set<monotoneSubset, SetSetLess> setOfMonotoneSubsets;

    public:
        DedekindInt();
        void generateMonotoneSubsets(size_t n);

    private:
        setOfMonotoneSubsets generate(
                setOfMonotoneSubsets const &m1, size_t n);

        monotoneSubset concatenate(
                monotoneSubset const &lhs,
                monotoneSubset const &rhs, size_t n);
};

std::ostream &operator<<(std::ostream &out, std::set<size_t> const &rhs);

std::ostream &operator<<(std::ostream &out,
    std::set<std::set<size_t>, SetLess> const &rhs);

std::ostream &operator<<(std::ostream &out,
    std::set<std::set<std::set<size_t>, SetLess>, SetSetLess> const &rhs);


#endif // end of guard DEDEKINDINT_H