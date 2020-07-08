
#include "ranges/type_map.h"

namespace MultiArrayTools
{
#define XCOMMAX() ,
#define include_type(t,n) template struct TypeMap<n>; \
    template struct NumTypeMap<t>;

#include "ranges/type_register.h"
    /*
    include_type(size_t,1)
    include_type(int,2)
    include_type(char,3)
    include_type(float,4)
    include_type(double,5)
    include_type(std::string,6)
    include_type(vector<size_t>,101)
    include_type(vector<int>,102)
    include_type(vector<double>,105)
    include_type(vector<std::string>,106)
    include_type(std::array<size_t XCOMMAX() 2>,201)
    include_type(std::array<int XCOMMAX() 2>,202)
    include_type(std::array<double XCOMMAX() 2>,205)
    include_type(std::array<size_t XCOMMAX() 3>,301)
    include_type(std::array<int XCOMMAX() 3>,302)
    include_type(std::array<double XCOMMAX() 3>,305)
    include_type(std::array<size_t XCOMMAX() 4>,401)
    include_type(std::array<int XCOMMAX() 4>,402)
    include_type(std::array<double XCOMMAX() 4>,405)
    include_type(std::array<int XCOMMAX() 5>,502)
    include_type(std::array<int XCOMMAX() 6>,602)
    include_type(std::array<int XCOMMAX() 7>,702)
    include_type(std::array<int XCOMMAX() 8>,802)
    include_type(std::array<int XCOMMAX() 9>,902)
    */
#undef include_type
#undef XCOMMAX

}
