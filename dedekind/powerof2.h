
#ifndef POWEROF2_H_
#define POWEROF2_H_

#include <cstddef>

namespace Dedekind
{

namespace Internal
{


template<size_t Number>
struct PowerOf2
{
	static size_t const value;
};

template<size_t Number>
size_t const PowerOf2<Number>::value((PowerOf2<Number - 1>::value << 1));


template<>
struct PowerOf2<0>
{
	static size_t const value;
};

size_t const PowerOf2<0>::value(1);



template<size_t Number>
struct LogOf2
{
	static size_t const value;
};

template<size_t Number>
size_t const LogOf2<Number>::value(LogOf2<(Number >> 1)>::value + 1);


template<>
struct LogOf2<1>
{
	static size_t const value;
};

size_t const LogOf2<1>::value(0);


} // namespace Internal

} // namespace Dedekind


#endif
