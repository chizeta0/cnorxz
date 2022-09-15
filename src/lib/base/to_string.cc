
#include "base/base.h"

namespace CNORXZ
{
    String ToString<String>::func(const String& a)
    {
	return a;
    }
    
    String ToString<DType>::func(const DType& a)
    {
	return a.str();
    }
}
