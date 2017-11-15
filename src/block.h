// -*- C++ -*-

#ifndef __ma_block_h__
#define __ma_block_h__

#include <cstdlib>
#include <vector>

#include "base_def.h"

namespace MultiArrayHelper
{

    enum class BlockType {
	INDEF = 0,
	BLOCK = 1,
	VALUE = 2,
	SPLIT = 3,
	RESULT = 4
    };
    
    // manage vectorization in the future !!
    
    template <typename T, class OpFunc, class BlockClass1, class BlockClass2>
    class BlockBinaryOp
    {
    public:
	BlockBinaryOp() = default;
	BlockResult<T> operator()(const BlockClass1& arg1, const BlockClass2& arg2);
    };

    template <typename T, class OpFunc, class BlockClass>
    class BlockBinaryOpSelf
    {
    public:
	BlockBinaryOpSelf(BlockResult<T>& res);
	void operator()(const BlockClass& arg);
    private:
	BlockResult<T>& mRes;
    };

    
    // EVERYTHING IN HERE MUST  N O T  BE VITUAL !!
    
    template <typename T>
    class BlockBase
    {
    public:
	DEFAULT_MEMBERS(BlockBase);
	BlockBase(size_t size);

	size_t size() const;

    protected:
	size_t mSize = 0;
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& out, const MBlock<T>& block)
    {
	out << block[0];
	for(size_t i = 1; i != block.size(); ++i){
	    out << ", " << block[i];
	}
	return out;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& out, const BlockResult<T>& block)
    {
	out << block[0];
	for(size_t i = 1; i != block.size(); ++i){
	    out << ", " << block[i];
	}
	return out;
    }

    
    template <typename T>
    class MutableBlockBase : public BlockBase<T>
    {
    public:

	DEFAULT_MEMBERS(MutableBlockBase);
	MutableBlockBase(size_t size);

    };
    
    template <typename T>
    class Block : public BlockBase<T>
    {
    public:
	DEFAULT_MEMBERS(Block);
	Block(const std::vector<T>& data, size_t begPos, size_t size, size_t stepSize);

	BlockType type() const;
	const T& operator[](size_t pos) const;
	Block& set(size_t npos);
	size_t stepSize() const;
	
    protected:
	const std::vector<T>* mData;
	const T* mBegPtr;
	size_t mStepSize;
    };

    template <typename T>
    class MBlock : public MutableBlockBase<T>
    {
    public:
	DEFAULT_MEMBERS(MBlock);
	MBlock(std::vector<T>& data, size_t begPos, size_t size, size_t stepSize);

	template <class BlockClass>
	MBlock& operator=(const BlockClass& in);
	
	BlockType type() const;
	const T& operator[](size_t pos) const;
	T& operator[](size_t pos);
	MBlock& set(size_t npos);
	size_t stepSize() const;
	
    protected:
	std::vector<T>* mData;
	T* mBegPtr;
	size_t mStepSize;
    };    
    
    template <typename T>
    class BlockResult : public MutableBlockBase<T>
    {
    public:
	typedef BlockBase<T> BB;
	
	DEFAULT_MEMBERS(BlockResult);
	
	BlockResult(size_t size);
	
	template <class BlockClass>
	BlockResult& operator=(const BlockClass& in);

	BlockResult& assign(size_t size, const T& val);
	
	BlockType type() const;
	const T& operator[](size_t pos) const;
	T& operator[](size_t i);
	BlockResult& set(size_t npos);
	size_t stepSize() const;
	
    protected:
	std::vector<T> mRes;
    };
    
} // end namespace MultiArrayHelper

#include "block.cc"

#endif
