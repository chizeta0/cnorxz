// -*- C++ -*-

#include "block.h"

namespace MultiArrayHelper
{

    /*********************
     *   BlockBinaryOp   *
     *********************/

    template <typename T, class OpFunc>
    BlockResult<T> BlockBinaryOp<T,OpFunc>::operator()(const BlockBase<T>& arg1,
						       const BlockBase<T>& arg2)
    {
	static OpFunc f;
	BlockResult<T> res(arg1.size());
	assert(arg1.size() == arg2.size());
	for(size_t i = 0; i != arg1.size(); ++i){
	    res[i] = f(arg1[i], arg2[i]);
	}
	return res;
    }
    
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
    BlockResult<T> BlockBase<T>::operate(const BlockBase<T>& in)
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
    BlockResult<T> BlockBase<T>::operator+(const BlockBase<T>& in)
    {
	return operate<std::plus<T> >(in);
    }

    template <typename T>
    BlockResult<T> BlockBase<T>::operator-(const BlockBase<T>& in)
    {
	return operate<std::minus<T> >(in);
    }
    
    template <typename T>
    BlockResult<T> BlockBase<T>::operator*(const BlockBase<T>& in)
    {
	return operate<std::multiplies<T> >(in);
    }
    
    template <typename T>
    BlockResult<T> BlockBase<T>::operator/(const BlockBase<T>& in)
    {
	return operate<std::divides<T> >(in);
    }

    /************************
     *   MutableBlockBase   *
     ************************/
    
    template <typename T>
    MutableBlockBase<T>::MutableBlockBase(size_t size) : BlockBase<T>(size) {}

    template <typename T>
    MutableBlockBase<T>& MutableBlockBase<T>::operator=(const BlockBase<T>& in)
    {
	for(size_t i = 0; i != BlockBase<T>::mSize; ++i){
	    (*this)[i] = in[i];
	}
	return *this;
    }
    
    /*************
     *   Block   *
     *************/

    template <typename T>
    Block<T>::Block(const std::vector<T>& data,
		    size_t begPos, size_t size) :
	BlockBase<T>(size),
	mData(&data),
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
    Block<T>& Block<T>::set(size_t npos)
    {
	mBegPtr = &(*mData)[npos];
	return *this;
    }

    template <typename T>
    size_t Block<T>::stepSize() const
    {
	return 1;
    }
    
    /**************
     *   MBlock   *
     **************/

    template <typename T>
    MBlock<T>::MBlock(std::vector<T>& data,
		      size_t begPos, size_t size) :
	MutableBlockBase<T>(size),
	mData(&data),
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
    MBlock<T>& MBlock<T>::set(size_t npos)
    {
	mBegPtr = &(*mData)[npos];
	return *this;
    }   

    template <typename T>
    size_t MBlock<T>::stepSize() const
    {
	return 1;
    }
    
    /******************
     *   BlockValue   *
     ******************/

    template <typename T>
    BlockValue<T>::BlockValue(const std::vector<T>& data,
			      size_t pos, size_t size) :
	BlockBase<T>(size),
	mData(&data),
	mVal(&data[pos]) {}

    template <typename T>
    BlockType BlockValue<T>::type() const
    {
	return BlockType::VALUE;
    }
    
    template <typename T>
    const T& BlockValue<T>::operator[](size_t i) const
    {
	
	return *mVal;
    }

    template <typename T>
    BlockValue<T>& BlockValue<T>::set(size_t npos)
    {
	mVal = &(*mData)[npos];
	return *this;
    }

    template <typename T>
    size_t BlockValue<T>::stepSize() const
    {
	return 0;
    }
    
    /*******************
     *   MBlockValue   *
     *******************/

    template <typename T>
    MBlockValue<T>::MBlockValue(std::vector<T>& data,
				size_t pos, size_t size) :
	MutableBlockBase<T>(size),
	mData(&data),
	mVal(&data[pos]) {}

    template <typename T>
    BlockType MBlockValue<T>::type() const
    {
	return BlockType::VALUE;
    }
    
    template <typename T>
    const T& MBlockValue<T>::operator[](size_t i) const
    {
	
	return *mVal;
    }

    template <typename T>
    T& MBlockValue<T>::operator[](size_t i)
    {
	
	return *mVal;
    }
    
    template <typename T>
    MBlockValue<T>& MBlockValue<T>::set(size_t npos)
    {
	mVal = &(*mData)[npos];
	return *this;
    }

    template <typename T>
    size_t MBlockValue<T>::stepSize() const
    {
	return 0;
    }
    
    /******************
     *   SplitBlock   *
     ******************/

    template <typename T>
    SplitBlock<T>::SplitBlock(const std::vector<T>& data, size_t begPos,
			      size_t stepSize, size_t size) :
	BlockBase<T>(size),
	mData(&data),
	mStepSize(stepSize),
	mBegPtr(&data[begPos]) {}

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
    SplitBlock<T>& SplitBlock<T>::set(size_t npos)
    {
	mBegPtr = &(*mData)[npos];
	return *this;
    }

    template <typename T>
    size_t SplitBlock<T>::stepSize() const
    {
	return mStepSize;
    }
    
    /*******************
     *   MSplitBlock   *
     *******************/

    template <typename T>
    MSplitBlock<T>::MSplitBlock(std::vector<T>& data, size_t begPos,
				size_t stepSize, size_t size) :
	MutableBlockBase<T>(size),
	mData(&data),
	mStepSize(stepSize),
	mBegPtr(&data[begPos]) {}

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
    MSplitBlock<T>& MSplitBlock<T>::set(size_t npos)
    {
	mBegPtr = &(*mData)[npos];
	return *this;
    }

    template <typename T>
    size_t MSplitBlock<T>::stepSize() const
    {
	return mStepSize;
    }
    
    /*******************
     *   BlockResult   *
     *******************/

    template <typename T>
    BlockResult<T>::BlockResult(size_t size) :
	MutableBlockBase<T>(size),
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
    BlockResult<T>& BlockResult<T>::set(size_t npos)
    {
	return *this;
    }

    template <typename T>
    size_t BlockResult<T>::stepSize() const
    {
	return 1;
    }
    
} // end namespace MultiArrayHelper
