#include "cnorxz.h"
#include "hl_cnorxz.h"

namespace CNORXZ
{
    template class HighLevelOp<OpCD,plusx<double,double>,2>;
    template class HighLevelOp<OpD,plusx<double,double>,2>;
}
