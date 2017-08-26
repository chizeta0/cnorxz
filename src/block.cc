// -*- C++ -*-

#include "block.h"

namespace MultiArrayHelper
{

    /*****************
     *   BlockBase   *
     *****************/
    
    template <typename T>
    BlockBase<T>::BlockBase(size_t size) : mSize(size) {}

    template <typename T>
    size_t BlockBase<T>::size() const
    {
	return mSize;
    }

    template <typename T>
    template <class OpFunction>
    BlockResult<T> BlockBase<T>::operate(const BlockBase& in)
    {
	assert(mSize == in.size());
	OpFunction f;
	BlockResult<T> res(mSize);
	for(size_t i = 0; i != mSize; ++i){
	    res[i] = f(this->operator[](i), in[i]);
	}
	return res;
    }

    /*************
     *   Block   *
     *************/

    template <typename T>
    Block<T>::Block(const std::vector<T>& data,
		    size_t begPos, size_t size) :
	BlockBase<T>(size),
	mBegPtr(data.data() + begPos) {}

    template <typename T>
    BlockType Block<T>::type() const
    {
	return BlockType::BLOCK;
    }
    
    template <typename T>
    const T& Block<T>::operator[](size_t i) const
    {
	return *(mBegPtr + i);
    }
    
    /******************
     *   BlockValue   *
     ******************/

    template <typename T>
    BlockValue<T>::BlockValue(const T& val, size_t size) :
	BlockBase<T>(size),
	mVal(val) {}

    template <typename T>
    BlockType BlockValue<T>::type() const
    {
	return BlockType::VALUE;
    }
    
    template <typename T>
    const T& BlockValue<T>::operator[](size_t i) const
    {
	return mVal;
    }
    
    /******************
     *   SplitBlock   *
     ******************/

    template <typename T>
    SplitBlock<T>::SplitBlock(std::vector<T*>&& begPtrVec) :
	BlockBase<T>(begPtrVec.size()),
	mBegPtr(begPtrVec) {}

    template <typename T>
    BlockType SplitBlock<T>::type() const
    {
	return BlockType::SPLIT;
    }
    
    template <typename T>
    const T& SplitBlock<T>::operator[](size_t i) const
    {
	return *(mBegPtrVec[i]);
    }
    
    /*******************
     *   BlockResult   *
     *******************/

    template <typename T>
    BlockResult<T>::BlockResult(size_t size) :
	BlockBase<T>(size),
	mRes(size) {}

    template <typename T>
    BlockType BlockResult<T>::type() const
    {
	return BlockType::RESULT;
    }
    
    template <typename T>
    const T& BlockResult<T>::operator[](size_t i) const
    {
	return mRes[i];
    }

    template <typename T>
    T& BlockResult<T>::operator[](size_t i)
    {
	return mRes[i];
    }


    
} // end namespace MultiArrayHelper
