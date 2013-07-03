
#ifndef BITSETLESS_H_
#define BITSETLESS_H_

#include <bitset>
#include <cstddef>


namespace Dedekind
{


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

template <size_t size>
bool bitsetLess(std::bitset<size> const &lhs, std::bitset<size> const &rhs)
{
	return lhs.to_ulong() > rhs.to_ulong();
}

} // namespace Dedekind


#endif
