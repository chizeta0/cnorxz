

#ifndef __bbase_def_h__
#define __bbase_def_h__

namespace MultiArrayHelper
{

    template <typename T, class OpFunc, class BlockClass1, class BlockClass2>
    class BlockBinaryOp;
    
    
    template <typename T, class BlockClass>
    class BlockBase;

    
    template <typename T, class BlockClass>
    class MutableBlockBase;

    
    template <typename T>
    class Block;

    
    template <typename T>
    class MBlock;

    
    template <typename T>
    class BlockResult;

    
    enum class BlockType {
	INDEF = 0,
	BLOCK = 1,
	VALUE = 2,
	SPLIT = 3,
	RESULT = 4,
	ARRAY = 5
    };


} // end namespace MultiArrayHelper

#endif
