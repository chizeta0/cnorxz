
#ifdef include_range_type
#define __cxz_incl_this__
#endif
#ifdef __cxz_single_range_h__
// singel_range is template which is specialized here
// therefore it must be defined before...
#define __cxz_incl_this__
#endif

#ifdef __cxz_incl_this__


#define __cxz_ranges_header__
//#ifndef __cxz_ranges_header__
//#define __cxz_ranges_header__

#include "null_range.h"
#include "spin_range.h"
#include "space_range.h"
#include "classic_range.h"

#undef __ranges_header__

#endif

#undef __incl_this__
