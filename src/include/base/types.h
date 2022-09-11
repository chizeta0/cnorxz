
#ifndef __cxz_types_h__
#define __cxz_types_h__

#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include <complex>
#include <array>
#include <map>
#include <typeinfo>

namespace CNORXZ
{
    /**********************
     *   standard types   *
     **********************/

    typedef std::intptr_t PtrId;
    typedef int32_t Int;
    typedef uint64_t SizeT;
    typedef double Double;
    typedef Double Real;
    typedef std::complex<Real> Complex;
    typedef std::string String;

    template <typename T, size_t N>
    using Arr = std::array<T,N>;
    
    template <typename T>
    using Sptr = std::shared_ptr<T>;

    template <typename... T>
    using Tuple = std::tuple<T...>;

    template <typename... T>
    using TupleElem = std::tuple_element<T...>;

    template <typename K, typename V>
    using Map = std::map<K,V>;

    typedef std::typeinfo TypeInfo;
    
    /*********************
     *   library types   *
     *********************/

    // definition: base/dtype.h
    class DType;

    // definition: memory/allocator.h
    template <typename T>
    class Allocator;

    // definition: ranges/range_base.h
    class RangeBase;

    /*********************
     *   derived types   *
     *********************/
    
    template <typename T>
    using Vector = std::vector<T,Allocator<T>>;
    
}

#endif
