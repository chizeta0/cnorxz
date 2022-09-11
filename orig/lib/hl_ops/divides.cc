#include "cnorxz.h"
#include "hl_cnorxz.h"

namespace CNORXZ
{
    template class HighLevelOp<OpCD,dividesx<double,double>,2>;
    template class HighLevelOp<OpD,dividesx<double,double>,2>;
}
