#include "cnorxz.h"
#include "hl_cnorxz.h"

namespace CNORXZ
{
    template class HighLevelOp<OpCD,minusx<double,double>,2>;
    template class HighLevelOp<OpD,minusx<double,double>,2>;
}
