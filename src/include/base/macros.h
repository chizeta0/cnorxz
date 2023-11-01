/**
   
   @file include/base/macros.h
   @brief useful macros

   Define useful macros that are used throughout this library for convenience,
   briefnes and/or debugging.

   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

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

#define INDEX_RANDOM_ACCESS_ITERATOR_DEFS(__meta_type__) typedef std::random_access_iterator_tag iterator_category; \
    typedef SizeT difference_type; \
    typedef __meta_type__ value_type; \
    typedef const __meta_type__* pointer; \
    typedef const __meta_type__& reference

#define CXZ_CVAL_FALSE static constexpr bool value = false
#define CXZ_CVAL_TRUE static constexpr bool value = true

#define IS_SAME(a,b) std::is_same<a,b>::value
#define IS_NOT_SAME(a,b) (not std::is_same<a,b>::value)

#ifdef HAVE_CEREAL
#define SERIALIZATION_FUNCTIONS template <class Archive> void save(Archive& ar, const std::uint32_t version) const; \
    template <class Archive> void load(Archive& ar, const std::uint32_t version)
#define SERIALIZATION_FUNCTIONS_NOPUB friend class cereal::access; \
    template <class Archive> void save(Archive& ar, const std::uint32_t version) const; \
    template <class Archive> void load(Archive& ar, const std::uint32_t version)
#else
#define SERIALIZATION_FUNCTIONS static const int v = 0
#define SERIALIZATION_FUNCTIONS_NOPUB static const int v = 0
#endif

#endif
