
#include "uint128.ih"

UInt128 &UInt128::operator+=(UInt128 const &other)
{
	if (d_lo > std::numeric_limits<unsigned long>::max() - other.d_lo)
	{
		++d_hi;
	}

	d_lo += other.d_lo;
	d_hi += other.d_hi;
	return *this;
}
