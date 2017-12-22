// -*- C++ -*-

#ifndef __ma_block_h__
#define __ma_block_h__

#include <cstdlib>
#include <vector>

#include "base_def.h"
#include "block/bbase_def.h"

namespace MultiArrayHelper
{
    
    // manage vectorization in the future !!
    
    template <typename T, class OpFunc, class BlockClass1, class BlockClass2>
    class BlockBinaryOp
    {
    public:
	BlockBinaryOp(BlockResult<T>& mRes);
	void operator()(const BlockClass1& arg1, const BlockClass2& arg2);
    private:
	BlockResult<T>& mRes;
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
	bool init() const;
	
    protected:
	size_t mSize = 0;
	bool mInit = false;
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& out, const Block<T>& block)
    {
	out << block[0];
	for(size_t i = 1; i != block.size(); ++i){
	    out << ", " << block[i];
	}
	return out;
    }
    
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
	Block(const T* data, size_t begPos, size_t size, size_t stepSize);

	BlockType type() const;
	const T& operator[](size_t pos) const;
	Block& set(size_t npos);
	size_t stepSize() const;
	
    protected:
	const T* mData;
	const T* mBegPtr;
	size_t mStepSize;
    };

    template <typename T>
    class MBlock : public MutableBlockBase<T>
    {
    public:
	DEFAULT_MEMBERS(MBlock);
	MBlock(T* data, size_t begPos, size_t size, size_t stepSize);

	template <class BlockClass>
	MBlock& operator=(const BlockClass& in);
	
	BlockType type() const;
	const T& operator[](size_t pos) const;
	T& operator[](size_t pos);
	MBlock& set(size_t npos);
	size_t stepSize() const;
	
    protected:
	T* mData;
	T* mBegPtr;
	size_t mStepSize;
    };    
    
    template <typename T>
    class BlockResult : public MutableBlockBase<T>
    {
    public:
	
	typedef BlockBase<T> BB;
	using BB::init;
	
	BlockResult();
	BlockResult(const BlockResult& in);
	BlockResult(BlockResult&& in);
	BlockResult& operator=(const BlockResult& in);
	BlockResult& operator=(BlockResult&& in);

	BlockResult(size_t size);

	~BlockResult();
	
	template <class BlockClass>
	BlockResult& operator=(const BlockClass& in);

	BlockResult& init(size_t size);
	
	BlockType type() const;
	const T& operator[](size_t pos) const;
	T& operator[](size_t i);
	BlockResult& set(size_t npos);
	size_t stepSize() const;
	
    protected:
	T* mResPtr = nullptr;
    };
    
} // end namespace MultiArrayHelper

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayHelper
{

    /*********************
     *   BlockBinaryOp   *
     *********************/

    template <typename T, class OpFunc, class BlockClass1, class BlockClass2>
    BlockBinaryOp<T,OpFunc,BlockClass1,BlockClass2>::BlockBinaryOp(BlockResult<T>& res) : mRes(res) {}
    
    template <typename T, class OpFunc, class BlockClass1, class BlockClass2>
    void BlockBinaryOp<T,OpFunc,BlockClass1,BlockClass2>::operator()(const BlockClass1& arg1,
								     const BlockClass2& arg2)
    {
	static OpFunc f;
	assert(mRes.init() and arg1.init() and arg2.init());
	assert(arg1.size() == arg2.size());
	for(size_t i = 0; i != arg1.size(); ++i){
	    mRes[i] = f(arg1[i], arg2[i]);
	}
    }

    template <typename T, class OpFunc, class BlockClass>
    BlockBinaryOpSelf<T,OpFunc,BlockClass>::BlockBinaryOpSelf(BlockResult<T>& res) : mRes(res) {}

    template <typename T, class OpFunc, class BlockClass>
    void BlockBinaryOpSelf<T,OpFunc,BlockClass>::operator()(const BlockClass& arg)
    {
	static OpFunc f;
	assert(mRes.init() and arg.init());
	assert(mRes.size() == arg.size());
	for(size_t i = 0; i != arg.size(); ++i){
	    mRes[i] = f(mRes[i], arg[i]);
	}
    }

    
    /*****************
     *   BlockBase   *
     *****************/
    
    template <typename T>
    BlockBase<T>::BlockBase(size_t size) : mSize(size), mInit(size != 0) {}

    template <typename T>
    size_t BlockBase<T>::size() const
    {
	return mSize;
    }

    template <typename T>
    bool BlockBase<T>::init() const
    {
	return mInit;
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
    Block<T>::Block(const T* data,
		    size_t begPos, size_t size, size_t stepSize) :
	BlockBase<T>(size),
	mData(data),
	mBegPtr(data + begPos),
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
	mBegPtr = mData + npos;
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
    MBlock<T>::MBlock(T* data,
		      size_t begPos, size_t size, size_t stepSize) :
	MutableBlockBase<T>(size),
	mData(data),
	mBegPtr(data + begPos),
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
	mBegPtr = mData + npos;
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
    BlockResult<T>::BlockResult() : MutableBlockBase<T>() {}

    template <typename T>
    BlockResult<T>::BlockResult(const BlockResult<T>& in) : MutableBlockBase<T>(in.size())
    {
	if(BB::mInit){
	    mResPtr = new T[BB::mSize];
	}
	for(size_t i = 0; i != BB::mSize; ++i){
	    mResPtr[i] = in.mResPtr[i];
	}
    }

    template <typename T>
    BlockResult<T>::BlockResult(BlockResult<T>&& in) : MutableBlockBase<T>(in.size())
    {
	if(BB::mInit){
	    mResPtr = in.mResPtr;
	}
	in.mSize = 0;
	in.mInit = false;
	in.mResPtr = nullptr;
    }

    template <typename T>
    BlockResult<T>& BlockResult<T>::operator=(const BlockResult<T>& in)
    {
	BB::mSize = in.size();
	BB::mInit = BB::mInit and BB::mSize != 0;
	if(BB::mInit){
	    mResPtr = new T[BB::mSize];
	}
	for(size_t i = 0; i != BB::mSize; ++i){
	    mResPtr[i] = in.mResPtr[i];
	}
	return *this;
    }

    template <typename T>
    BlockResult<T>& BlockResult<T>::operator=(BlockResult<T>&& in)
    {
	BB::mSize = in.size();
	BB::mInit = BB::mInit and BB::mSize != 0;
	if(BB::mInit){
	    mResPtr = in.mResPtr;
	}
	in.mSize = 0;
	in.mInit = false;
	in.mResPtr = nullptr;
	return *this;
    }

    template <typename T>
    BlockResult<T>::BlockResult(size_t size) :
	MutableBlockBase<T>(size)
    {
	if(BB::mInit){
	    mResPtr = new T[BB::mSize];
	}
	for(size_t i = 0; i != BB::mSize; ++i){
	    mResPtr[i] = static_cast<T>( 0 );
	}
    }

    template <typename T>
    BlockResult<T>::~BlockResult()
    {
	delete[] mResPtr;
	mResPtr = nullptr;
	BB::mInit = false;
	BB::mSize = 0;
    }
    
    template <typename T>
    template <class BlockClass>
    BlockResult<T>& BlockResult<T>::operator=(const BlockClass& in)
    {
	assert(BB::mInit);
	assert(BB::mSize == in.size());
	for(size_t i = 0; i != BB::mSize; ++i){
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
	return mResPtr[i];
    }

    template <typename T>
    T& BlockResult<T>::operator[](size_t i)
    {
	return mResPtr[i];
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
    BlockResult<T>& BlockResult<T>::init(size_t size)
    {
	BB::mSize = size;
	delete[] mResPtr;
	if(BB::mSize != 0){
	    BB::mInit = true;
	    mResPtr = new T[BB::mSize];
	}
	else {
	    BB::mInit = false;
	}
	for(size_t i = 0; i != BB::mSize; ++i){
	    mResPtr[i] = static_cast<T>( 0 );
	}
	return *this;
    }
    
} // end namespace MultiArrayHelper

#endif
