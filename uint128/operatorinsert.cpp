
#include "uint128.ih"

// http://stackoverflow.com/questions/4361441/c-print-a-biginteger-in-base-10

namespace Dedekind
{
	std::ostream &operator<<(std::ostream &out, UInt128 const &uint128)
	{
		// // get current precision, set to fixed 0, print, set back to default
		// streamsize old = cout.precision();
		// out.precision(0);

		// out << std::fixed << uint128.d_hi * pow(2, 64) + uint128.d_lo;

		// out.unsetf(ios_base::floatfield);
		// out.precision(old);


		size_t d[39] = {0}; // a 128 bit number has at most 39 digits

		// starting at the highest, for each bit
		for (int iter = 63; iter != -1; --iter)
		{
			// increase the lowest digit if this bit is set
			if ((uint128.d_hi >> iter) & 1)
			{
				d[0]++;
			}

			// multiply by 2. now the highest bit will reach the value 2^63
			for (size_t idx = 0; idx < 39; ++idx)
			{
				d[idx] *= 2;
			}

			// handle carries/overflow
			for (size_t idx = 0; idx < 38; ++idx)
			{
				d[idx + 1] += d[idx] / 10;
				d[idx] %= 10;
			}
		}

		for (int iter = 63; iter > -1; --iter)
		{
			// increase the lowest digit if this bit is set
			if ((uint128.d_lo >> iter) & 1)
			{
				d[0]++;
			}

			// only multiply if more bits will follow
			if (iter > 0)
			{
				for (size_t idx = 0; idx < 39; ++idx)
				{
					d[idx] *= 2;
				}
			}

			// handle carries/overflow
			for (size_t idx = 0; idx < 38; ++idx)
			{
				d[idx + 1] += d[idx] / 10;
				d[idx] %= 10;
			}
		}

		// find highest digit to be inserted in out
		int idx;
		for (idx = 38; idx > 0; --idx)
		{
			if (d[idx] > 0)
			{
				break;
			}
		}

		// insert from here
		for (; idx > -1; --idx)
		{
			out << d[idx];
		}

		return out;
	}
}
