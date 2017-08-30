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
	    res[i] = f((*this)[i], in[i]);
	}
	return res;
    }

    template <typename T>
    BlockResult<T> BlockBase<T>::operator+(const BlockBase& in)
    {
	return operate<std::plus<T> >(in);
    }

    template <typename T>
    BlockResult<T> BlockBase<T>::operator-(const BlockBase& in)
    {
	return operate<std::minus<T> >(in);
    }
    
    template <typename T>
    BlockResult<T> BlockBase<T>::operator*(const BlockBase& in)
    {
	return operate<std::multiplies<T> >(in);
    }
    
    template <typename T>
    BlockResult<T> BlockBase<T>::operator/(const BlockBase& in)
    {
	return operate<std::divides<T> >(in);
    }

    /************************
     *   MutableBlockBase   *
     ************************/
    
    template <typename T>
    MutableBlockBase<T>::MutableBlockBase(size_t size) : BlockBase<T>(size) {}
    
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

    template <typename T>
    Block<T>& Block<T>::set(const T* nbeg)
    {
	mBegPtr = nbeg;
	return *this;
    }

    /**************
     *   MBlock   *
     **************/

    template <typename T>
    MBlock<T>::MBlock(const std::vector<T>& data,
		    size_t begPos, size_t size) :
	MutableBlockBase<T>(size),
	mBegPtr(data.data() + begPos) {}

    template <typename T>
    BlockType MBlock<T>::type() const
    {
	return BlockType::BLOCK;
    }
    
    template <typename T>
    const T& MBlock<T>::operator[](size_t i) const
    {
	return *(mBegPtr + i);
    }

    template <typename T>
    T& MBlock<T>::operator[](size_t i)
    {
	return *(mBegPtr + i);
    }
    
    template <typename T>
    MBlock<T>& MBlock<T>::set(const T* nbeg)
    {
	mBegPtr = nbeg;
	return *this;
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

    template <typename T>
    BlockValue<T>& BlockValue<T>::set(const T* nbeg)
    {
	mVal = *nbeg;
	return *this;
    }

    /*******************
     *   MBlockValue   *
     *******************/

    template <typename T>
    MBlockValue<T>::MBlockValue(const T& val, size_t size) :
	BlockBase<T>(size),
	mVal(val) {}

    template <typename T>
    BlockType MBlockValue<T>::type() const
    {
	return BlockType::VALUE;
    }
    
    template <typename T>
    const T& MBlockValue<T>::operator[](size_t i) const
    {
	return mVal;
    }

    template <typename T>
    T& MBlockValue<T>::operator[](size_t i)
    {
	return mVal;
    }
    
    template <typename T>
    MBlockValue<T>& MBlockValue<T>::set(const T* nbeg)
    {
	mVal = *nbeg;
	return *this;
    }

    /******************
     *   SplitBlock   *
     ******************/

    template <typename T>
    SplitBlock<T>::SplitBlock(const std::vector<T>& data, size_t begPos,
			      size_t stepSize, size_t size) :
	BlockBase<T>(size),
	mStepSize(stepSize),
	mBegPtr(data.data() + begPos) {}

    template <typename T>
    BlockType SplitBlock<T>::type() const
    {
	return BlockType::SPLIT;
    }

    template <typename T>
    const T& SplitBlock<T>::operator[](size_t pos) const
    {
	return *(mBegPtr + pos*mStepSize);
    }	
    
    template <typename T>
    SplitBlock<T>& SplitBlock<T>::set(const T* nbeg)
    {
	mBegPtr = nbeg;
	return *this;
    }

    /*******************
     *   MSplitBlock   *
     *******************/

    template <typename T>
    MSplitBlock<T>::MSplitBlock(const std::vector<T>& data, size_t begPos,
			      size_t stepSize, size_t size) :
	BlockBase<T>(size),
	mStepSize(stepSize),
	mBegPtr(data.data() + begPos) {}

    template <typename T>
    BlockType MSplitBlock<T>::type() const
    {
	return BlockType::SPLIT;
    }

    template <typename T>
    const T& MSplitBlock<T>::operator[](size_t pos) const
    {
	return *(mBegPtr + pos*mStepSize);
    }	

    template <typename T>
    T& MSplitBlock<T>::operator[](size_t pos)
    {
	return *(mBegPtr + pos*mStepSize);
    }	
    
    template <typename T>
    MSplitBlock<T>& MSplitBlock<T>::set(const T* nbeg)
    {
	mBegPtr = nbeg;
	return *this;
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

    template <typename T>
    BlockResult<T>& BlockResult<T>::set(const T* nbeg)
    {
	return *this;
    }
    
} // end namespace MultiArrayHelper
