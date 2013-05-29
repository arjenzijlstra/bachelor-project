
#include "uint128.ih"

namespace Dedekind
{
	std::ostream &operator<<(std::ostream &out, UInt128 const &uint128)
	{
		if (uint128.d_hi != 0)
		{
			 // get current precision, set to fixed 0, print, set back to default
			streamsize old = cout.precision();
			out.precision(0);

			out << std::fixed << uint128.d_hi * pow(2, 64) + uint128.d_lo;

			out.unsetf(ios_base::floatfield);
			out.precision(old);
		}
		else
		{
			out << uint128.d_lo;
		}

		return out;
	}
}
