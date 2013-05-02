
#include "dedekindint.ih"

DedekindInt::DedekindInt()
{
	d_base = DedekindInt::setOfMonotoneSubsets({{ }, {{ }}});
}
