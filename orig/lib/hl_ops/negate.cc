#include "cnorxz.h"
#include "hl_cnorxz.h"

namespace CNORXZ
{
    template class HighLevelOp<OpCD,negate<double>,1>;
    template class HighLevelOp<OpD,negate<double>,1>;
}
