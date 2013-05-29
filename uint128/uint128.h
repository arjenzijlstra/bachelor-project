
#ifndef DEDEKIND_UINT_
#define DEDEKIND_UINT_

#include <cstdint>
#include <iosfwd>

namespace Dedekind
{
	class UInt128
	{
		uint_fast64_t d_hi;
		uint_fast64_t d_lo;

		public:
			friend std::ostream &operator<<(std::ostream &out,
					UInt128 const &uint128);

			UInt128() = default;
			UInt128(UInt128 const &other) = default;
			UInt128(uint_fast64_t lo, uint_fast64_t hi = 0);

			UInt128 &operator+=(uint_fast64_t other);
			UInt128 &operator+=(UInt128 const &other);

			uint_fast64_t hi() const;
			uint_fast64_t lo() const;
	};


	inline UInt128 operator+(UInt128 const &lhs, UInt128 const &rhs)
	{
		UInt128 tmp(lhs);
		return tmp += rhs;
	}

	inline uint_fast64_t UInt128::hi() const
	{
		return d_hi;
	}

	inline uint_fast64_t UInt128::lo() const
	{
		return d_lo;
	}
}

#endif
