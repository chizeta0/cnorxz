
#ifndef __operation_utils_h__
#define __operation_utils_h__

#include <utility>
#include <vector>
#include <memory>
#include <map>

#include "block/block.h"
#include "ranges/vindex_base.h"

namespace MultiArrayTools
{
       
    // <block type, step size within actual instance>
    typedef std::pair<BlockType,size_t> BTSS;

    
    void seekIndexInst(std::shared_ptr<VIWB> i, std::vector<std::shared_ptr<VIWB> >& ivec);
    
    BTSS getBlockType(std::shared_ptr<VIWB> i,
		      std::shared_ptr<VIWB> j,
		      bool first, size_t higherStepSize = 1);


    size_t getBTNum(const std::vector<BTSS>& mp, BlockType bt);

    void minimizeAppearanceOfType(std::map<std::shared_ptr<VIWB>, std::vector<BTSS> >& mp,
				  BlockType bt);

    
} // end namespace MultiArrayTools

#endif
