
#include "uint128.ih"

UInt128 &UInt128::operator+=(uint_fast64_t other)
{
	if (d_lo > std::numeric_limits<unsigned long>::max() - other)
	{
		++d_hi;
	}

	d_lo += other;
	return *this;
}
