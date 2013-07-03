
#ifndef BITSETOPERLEQ_H_
#define BITSETOPERLEQ_H_

#include <bitset>
#include <cstddef>


namespace Dedekind
{


template <size_t size>
bool operator<=(std::bitset<size> lhs, std::bitset<size> const &rhs)
{
	return (lhs.flip() | rhs).all();
}


} // namespace Dedekind


#endif
