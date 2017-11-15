// -*- C++ -*-

#include "block.h"

namespace MultiArrayHelper
{

    /*********************
     *   BlockBinaryOp   *
     *********************/

    template <typename T, class OpFunc, class BlockClass1, class BlockClass2>
    BlockResult<T>
    BlockBinaryOp<T,OpFunc,BlockClass1,BlockClass2>::operator()(const BlockClass1& arg1,
								const BlockClass2& arg2)
    {
	static OpFunc f;
	BlockResult<T> res(arg1.size());
	assert(arg1.size() == arg2.size());
	for(size_t i = 0; i != arg1.size(); ++i){
	    res[i] = f(arg1[i], arg2[i]);
	}
	return res;
    }

    template <typename T, class OpFunc, class BlockClass>
    BlockBinaryOpSelf<T,OpFunc,BlockClass>::BlockBinaryOpSelf(BlockResult<T>& res) : mRes(res) {}

    template <typename T, class OpFunc, class BlockClass>
    void BlockBinaryOpSelf<T,OpFunc,BlockClass>::operator()(const BlockClass& arg)
    {
	static OpFunc f;
	if(mRes.size() == 0) { mRes.assign(arg.size(), static_cast<T>(0)); }
	assert(mRes.size() == arg.size());
	for(size_t i = 0; i != arg.size(); ++i){
	    mRes[i] = f(mRes[i], arg[i]);
	}
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
		    size_t begPos, size_t size, size_t stepSize) :
	BlockBase<T>(size),
	mData(&data),
	mBegPtr(data.data() + begPos),
	mStepSize(stepSize) {}

    template <typename T>
    BlockType Block<T>::type() const
    {
	return mStepSize == 0 ? BlockType::VALUE :
	    ( mStepSize == 1 ? BlockType::BLOCK : BlockType::SPLIT );
    }
    
    template <typename T>
    const T& Block<T>::operator[](size_t i) const
    {
	
	return *(mBegPtr + i * mStepSize);
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
		      size_t begPos, size_t size, size_t stepSize) :
	MutableBlockBase<T>(size),
	mData(&data),
	mBegPtr(data.data() + begPos),
	mStepSize(stepSize) {}

    template <typename T>
    template <class BlockClass>
    MBlock<T>& MBlock<T>::operator=(const BlockClass& in)
    {
	for(size_t i = 0; i != BlockBase<T>::mSize; ++i){
	    (*this)[i] = in[i];
	}
	return *this;
    }
   
    template <typename T>
    BlockType MBlock<T>::type() const
    {
	return mStepSize == 0 ? BlockType::VALUE :
	    ( mStepSize == 1 ? BlockType::BLOCK : BlockType::SPLIT );
    }
    
    template <typename T>
    const T& MBlock<T>::operator[](size_t i) const
    {
	
	return *(mBegPtr + i * mStepSize);
    }

    template <typename T>
    T& MBlock<T>::operator[](size_t i)
    {
	
	return *(mBegPtr + i * mStepSize);
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
    
    /*******************
     *   BlockResult   *
     *******************/

    template <typename T>
    BlockResult<T>::BlockResult(size_t size) :
	MutableBlockBase<T>(size),
	mRes(size) {}

    template <typename T>
    template <class BlockClass>
    BlockResult<T>& BlockResult<T>::operator=(const BlockClass& in)
    {
	//CHECK;
	for(size_t i = 0; i != BlockBase<T>::mSize; ++i){
	    (*this)[i] = in[i];
	}
	return *this;
    }
    
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

    template <typename T>
    BlockResult<T>& BlockResult<T>::assign(size_t size, const T& val)
    {
	BB::mSize = size;
	mRes.assign(BB::mSize, val);
	return *this;
    }
    
} // end namespace MultiArrayHelper
