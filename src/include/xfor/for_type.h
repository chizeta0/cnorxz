
#ifndef __cxz_for_type_h__
#define __cxz_for_type_h__

namespace CNORXZInternal
{
    
    enum class ForType {
	DEFAULT = 0,
	HIDDEN = 1	
    };

    template <class IndexClass, class Expr, ForType FT = ForType::DEFAULT, size_t DIV = 1>
    class For;

    template <class IndexClass, class Expr, size_t DIV = 1>
    class PFor;

    
} // end namespace CNORXZInternal


#endif
