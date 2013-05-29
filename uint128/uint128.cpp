
#include "uint128.ih"

namespace Dedekind
{
	UInt128::UInt128(uint_fast64_t lo, uint_fast64_t hi)
	:
		d_hi(hi),
		d_lo(lo)
	{
	}
}
