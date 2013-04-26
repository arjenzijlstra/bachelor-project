
#ifndef DEDEKINDBASE_H_
#define DEDEKINDBASE_H_

#include <set>

template<typename Type, typename Less = std::less<Type>,
        typename Alloc = std::allocator<Type>>
class DedekindBase
{
    protected:
        size_t d_number;  // current number of Dedekind
        size_t d_size;  // number of monotone subsets
        std::set<Type, Less, Alloc> d_base;  // monotone subsetbase

    public:
        virtual void generateMonotoneSubsets(size_t n) = 0;
        
        std::set<Type, Less, Alloc> const &base() const
        {
            return d_base;
        }

    // private:
    //     virtual std::set<Type, Less, Alloc> generate(
    //             std::set<Type, Less, Alloc> const &m1, size_t n = 0) = 0;

    //     virtual Type concatenate(Type const &lhs,
    //             Type const &rhs, size_t n = 0) = 0;

};


#endif // end of guard DEDEKINDBASE_H_

