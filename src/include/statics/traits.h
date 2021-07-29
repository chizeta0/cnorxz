
#include "base_def.h"

namespace CNORXZ
{

    template <typename T>
    struct IsArray
    {
        static constexpr bool VALUE = false;
    };

#define add_array_trait(name) template <typename... X> struct IsArray<##name<X...>> { \
        static constexpr bool VALUE = true; \
    }

    add_array_trait(ArrayBase);
    add_array_trait(MutableArrayBase);
    add_array_trait(Array);
    add_array_trait(FunctionalArray);
    add_array_trait(Slice);
    add_array_trait(ConstSlice);

#undef add_array_trait

}
