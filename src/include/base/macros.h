
#ifndef __cxz_macros_h__
#define __cxz_macros_h__

#include <iostream>

#ifndef CHECK
#define CHECK std::cout << __FILE__ << ": @" << __LINE__ << " in " << __func__ << std::endl;
#endif
#ifndef VCHECK
#define VCHECK(a) std::cout << __FILE__ << ": @" << __LINE__ \
  << " in " << __func__ << ": " << #a << " = " << a << std::endl;
#endif

#define DEFAULT_C(__class_name__) __class_name__() = default
#define DEFAULT_COPY_C(__class_name__) __class_name__(const __class_name__& a) = default
#define DEFAULT_COPY_A(__class_name__) __class_name__& operator=(const __class_name__& a) = default
#define DEFAULT_MOVE_C(__class_name__) __class_name__(__class_name__&& a) = default
#define DEFAULT_MOVE_A(__class_name__) __class_name__& operator=(__class_name__&& a) = default
#define DEFAULT_COPY(__class_name__) DEFAULT_COPY_C(__class_name__); DEFAULT_COPY_A(__class_name__)
#define DEFAULT_MOVE(__class_name__) DEFAULT_MOVE_C(__class_name__); DEFAULT_MOVE_A(__class_name__)
#define DEFAULT_MEMBERS_X(__class_name__) DEFAULT_COPY(__class_name__); DEFAULT_MOVE(__class_name__)
#define DEFAULT_MEMBERS(__class_name__) DEFAULT_C(__class_name__); DEFAULT_MEMBERS_X(__class_name__)

#define SP_DEFAULT_C(__spec__,__class_name__) __spec__ __class_name__() = default
#define SP_DEFAULT_COPY_C(__spec__,__class_name__) __spec__ __class_name__(const __class_name__& a) = default
#define SP_DEFAULT_COPY_A(__spec__,__class_name__) __spec__ __class_name__& operator=(const __class_name__& a) = default
#define SP_DEFAULT_MOVE_C(__spec__,__class_name__) __spec__ __class_name__(__class_name__&& a) = default
#define SP_DEFAULT_MOVE_A(__spec__,__class_name__) __spec__ __class_name__& operator=(__class_name__&& a) = default
#define SP_DEFAULT_COPY(__spec__,__class_name__) SP_DEFAULT_COPY_C(__spec__,__class_name__); SP_DEFAULT_COPY_A(__spec__,__class_name__)
#define SP_DEFAULT_MOVE(__spec__,__class_name__) SP_DEFAULT_MOVE_C(__spec__,__class_name__); SP_DEFAULT_MOVE_A(__spec__,__class_name__)
#define SP_DEFAULT_MEMBERS_X(__spec__,__class_name__) SP_DEFAULT_COPY(__spec__,__class_name__); SP_DEFAULT_MOVE(__spec__,__class_name__)
#define SP_DEFAULT_MEMBERS(__spec__,__class_name__) SP_DEFAULT_C(__spec__,__class_name__); SP_DEFAULT_MEMBERS_X(__spec__,__class_name__)

#define CXZ_CVAL_FALSE static constexpr bool value = false
#define CXZ_CVAL_TRUE static constexpr bool value = true

#endif
