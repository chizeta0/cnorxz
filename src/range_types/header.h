
#ifdef include_range_type
#define __incl_this__
#endif
#ifdef __single_range_h__
// singel_range is template which is specialized here
// therefore it must be defined before...
#define __incl_this__
#endif

#ifdef __incl_this__

#ifndef __ranges_header__
#define __ranges_header__

#include "spin_range.h"

#endif
#endif

#undef __incl_this__
