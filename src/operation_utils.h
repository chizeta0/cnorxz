
#ifndef __operation_utils_h__
#define __operation_utils_h__

#include <utility>
#include <vector>
#include <memory>
#include <map>

#include "block/block.h"
#include "ranges/vindex_base.h"

#include "ranges/index_info.h"

namespace MultiArrayTools
{

    namespace
    {
	using namespace MultiArrayHelper;
    }
    
    // <block type, step size within actual instance>
    typedef std::pair<BlockType,size_t> BTSS;

    
    void seekIndexInst(std::shared_ptr<VIWB> i, std::vector<std::shared_ptr<VIWB> >& ivec);

    void seekIndexInst(const IndexInfo* i, std::vector<const IndexInfo*>& ivec);
    
    BTSS getBlockType(std::shared_ptr<VIWB> i,
		      std::shared_ptr<VIWB> j,
		      bool first, size_t higherStepSize = 1);

    BTSS getBlockType(const IndexInfo* i,
		      const IndexInfo* j,
		      bool first, size_t higherStepSize = 1);


    size_t getBTNum(const std::vector<BTSS>& mp, BlockType bt);

    void minimizeAppearanceOfType(std::map<std::shared_ptr<VIWB>, std::vector<BTSS> >& mp,
				  BlockType bt);

    void minimizeAppearanceOfType(std::map<const IndexInfo*, std::vector<BTSS> >& mp,
				  BlockType bt);

    
    
} // end namespace MultiArrayTools

#endif
