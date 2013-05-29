
#include "uint128.ih"

std::ostream &operator<<(std::ostream &out, UInt128 const &uint128)
{
	if (uint128.d_hi != 0)
	{
		out.precision(0);
		out << std::fixed << uint128.d_hi * pow(2, 64) + uint128.d_lo;
	}
	else
	{
		out << uint128.d_lo;
	}

	return out;
}
