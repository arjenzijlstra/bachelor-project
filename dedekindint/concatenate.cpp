
#include "dedekindint.ih"

DedekindInt::monotoneSubset DedekindInt::concatenate(
        DedekindInt::monotoneSubset const &lhs,
        DedekindInt::monotoneSubset const &rhs, size_t n)
{
    DedekindInt::monotoneSubset result;
    for (set<set<size_t>>::iterator iter = rhs.begin();
            iter != rhs.end();
                ++iter)
    {
        set<size_t> tmp(*iter);
        tmp.insert(n);
        result.insert(tmp);
    }

    return (lhs + result);
}