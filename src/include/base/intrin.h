// -*- C++ -*-
/**
   @file include/base/intrin.h
   @brief Definitions related to intrinsics
 **/

#ifndef __cxz_intrin_h__
#define __cxz_intrin_h__

#define MAX_VSIZE 1

// AVX:

#define AVX_BITS 256
#define AVX_VSIZE (AVX_BITS/8)
#ifdef CXZ_HAVE_AVX
#undef MAX_VSIZE
#define MAX_VSIZE AVX_VSIZE
#endif

// ...:

// ...

#endif
