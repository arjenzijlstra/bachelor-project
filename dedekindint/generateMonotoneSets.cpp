
#include "dedekindint.ih"

void DedekindInt::generateMonotoneSubsets(size_t n)
{
    for (size_t iter = 0; iter != n; ++iter)
    {
        d_base = generate(d_base, iter);
    }
    d_size = d_base.size();
    d_number = n;
}