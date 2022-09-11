#include "cnorxz.h"
#include "hl_cnorxz.h"

namespace CNORXZ
{
    template class HighLevelOp<OpCD,x_exp<double>,1>;
    template class HighLevelOp<OpD,x_exp<double>,1>;
    template HighLevelOpHolder<OpCD> hl_exp (const HighLevelOpHolder<OpCD>& in);
    template HighLevelOpHolder<OpD> hl_exp (const HighLevelOpHolder<OpD>& in);
}
