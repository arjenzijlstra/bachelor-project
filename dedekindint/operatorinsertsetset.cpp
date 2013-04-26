
#include "dedekindint.ih"

ostream &operator<<(ostream &out,
        set<set<size_t>, SetLess> const &rhs)
{
    for (auto iter = rhs.begin(); iter != rhs.end(); ++iter)
    {
        if ((*iter).size() == 0)
            out << "e ";
        else
            out << *iter;
    }
    out << '\n';
    return out;
}
