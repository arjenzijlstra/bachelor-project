
#include "dedekindint.ih"

DedekindInt::setOfMonotoneSubsets DedekindInt::generate(
    DedekindInt::setOfMonotoneSubsets const &m1, size_t n)
{
    setOfMonotoneSubsets m2;
    size_t mn = 0;

    for (auto iter = m1.begin();
            iter != m1.end();
                ++iter)
    {
        for (auto iter2 = m1.begin();
                iter2 != m1.end();
                    ++iter2)
        {
            if (*iter <= *iter2)
            {
                set<set<size_t>, SetLess> tmp = concatenate(*iter, *iter2, n);
                m2.insert(tmp);
                ++mn;
            }
        }
    }

    return m2;
}