
#ifndef __for_type_h__
#define __for_type_h__

namespace MultiArrayHelper
{
    
    enum class ForType {
	DEFAULT = 0,
	HIDDEN = 1	
    };

    template <class IndexClass, class Expr, ForType FT = ForType::DEFAULT>
    class For;

    
} // end namespace MultiArrayHelper


#endif
