#include "cnorxz.h"
#include "hl_cnorxz.h"

namespace CNORXZ
{
    template class HighLevelOp<OpCD,multipliesx<double,double>,2>;
    template class HighLevelOp<OpD,multipliesx<double,double>,2>;
}
