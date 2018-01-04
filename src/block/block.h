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
	BlockResult<T>& operator()(const BlockClass1& arg1, const BlockClass2& arg2);
    private:
	BlockResult<T>& mRes;
    };

    template <typename T, class OpFunc, class BlockClass>
    class BlockBinaryOpSelf
    {
    public:
	BlockBinaryOpSelf(BlockResult<T>& res);
	BlockResult<T>& operator()(const BlockClass& arg);
    private:
	BlockResult<T>& mRes;
    };

    template <typename T, class OpFunc, class BlockClass>
    class BlockContraction
    {
    public:
	BlockContraction(T& res);
	T& operator()(const BlockClass& arg);
    private:
	T& mRes;
    };
    
    // EVERYTHING IN HERE MUST  N O T  BE VITUAL !!
    
    template <typename T, class BlockClass>
    class BlockBase
    {
    public:

	const BlockClass& THIS() const { return static_cast<BlockClass const&>(*this); }
	BlockClass& THIS() { return static_cast<BlockClass&>(*this); }

	static BlockType sType() { return BlockClass::sType(); }
	
	size_t size() const;
	bool init() const;
	
	BlockType type() const { return THIS().type(); }
	const T& operator[](size_t pos) const { return THIS()[pos]; }
	BlockClass& set(size_t npos) { return THIS().set(npos); }
	size_t stepSize() const { return THIS().stepSize(); }
	
    private:

	friend BlockClass;
	friend MutableBlockBase<T,BlockClass>;
	
	DEFAULT_MEMBERS(BlockBase);
	BlockBase(size_t size);
	
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

    
    template <typename T, class BlockClass>
    class MutableBlockBase : public BlockBase<T,BlockClass>
    {
    public:

	typedef BlockBase<T,BlockClass> BB;
	
	T& operator[](size_t pos) { return BB::THIS()[pos]; }
	
    private:

	friend BlockClass;
	
	DEFAULT_MEMBERS(MutableBlockBase);
	MutableBlockBase(size_t size);

    };
    
    template <typename T>
    class Block : public BlockBase<T,Block<T> >
    {
    public:
	typedef BlockBase<T,Block<T> > BB;

	static BlockType sType() { return BlockType::BLOCK; }
	
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

    template <class BlockClass>
    class BlockArray : public BlockBase<BlockClass,BlockArray<BlockClass> >
    {

	typedef BlockBase<BlockClass,BlockArray<BlockClass> > BB;

	static BlockType sType() { return BlockType::ARRAY; }
	
	DEFAULT_MEMBERS(BlockArray);

	BlockArray(const BlockClass& block, size_t size, size_t stepSize);

	BlockType type() const;
	BlockClass& operator[](size_t pos);
	BlockArray& set(size_t npos);
	size_t stepSize() const;
	
    protected:
	const BlockClass& mBlock;
	size_t mStepSize; // total stepSize !!
    };
    
    
    template <typename T>
    class MBlock : public MutableBlockBase<T,MBlock<T> >
    {
    public:

	typedef BlockBase<T,MBlock<T> > BB;

	static BlockType sType() { return BlockType::BLOCK; }
	
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
    class BlockResult : public MutableBlockBase<T,BlockResult<T> >
    {
    public:
	
	typedef BlockBase<T,BlockResult<T> > BB;
	using BB::init;

	static BlockType sType() { return BlockType::RESULT; }
	
	BlockResult();
	BlockResult(const BlockResult& in);
	BlockResult(BlockResult&& in);
	BlockResult& operator=(const BlockResult& in);
	BlockResult& operator=(BlockResult&& in);

	template <typename... ArgTypes>
	BlockResult(size_t size, const ArgTypes&... args);
	
	~BlockResult();
	
	template <class BlockClass>
	BlockResult& operator=(const BlockClass& in);

	template <typename... Args>
	BlockResult& init(size_t size, const Args&... args);
	
	BlockType type() const;
	const T& operator[](size_t pos) const;
	T& operator[](size_t i);
	BlockResult& set(size_t npos);
	size_t stepSize() const;
	
    protected:
	T* mResPtr = nullptr;
	T* mBegPtr = nullptr;
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
    BlockResult<T>& BlockBinaryOp<T,OpFunc,BlockClass1,BlockClass2>::operator()(const BlockClass1& arg1,
								     const BlockClass2& arg2)
    {
	static OpFunc f;
	//assert(mRes.init() and arg1.init() and arg2.init());
	//assert(arg1.size() == arg2.size());
	for(size_t i = 0; i != arg1.size(); ++i){
	    mRes[i] = f(arg1[i], arg2[i]);
	}
	return mRes;
    }

    template <typename T, class OpFunc, class BlockClass>
    BlockBinaryOpSelf<T,OpFunc,BlockClass>::BlockBinaryOpSelf(BlockResult<T>& res) : mRes(res) {}

    template <typename T, class OpFunc, class BlockClass>
    BlockResult<T>& BlockBinaryOpSelf<T,OpFunc,BlockClass>::operator()(const BlockClass& arg)
    {
	static OpFunc f;
	//assert(mRes.init() and arg.init());
	//assert(mRes.size() == arg.size());
	for(size_t i = 0; i != arg.size(); ++i){
	    mRes[i] = f(mRes[i], arg[i]);
	}
	return mRes;
    }

    template <typename T, class OpFunc, class BlockClass>
    BlockContraction<T,OpFunc,BlockClass>::BlockContraction(T& res) : mRes(res) {}

    template <typename T, class OpFunc, class BlockClass>
    T& BlockContraction<T,OpFunc,BlockClass>::operator()(const BlockClass& arg)
    {
	static OpFunc f;

	for(size_t i = 0; i != arg.size(); ++i){
	    mRes = f(mRes, arg[i]);
	}
	return mRes;
    }

    
    /*****************
     *   BlockBase   *
     *****************/
    
    template <typename T, class BlockClass>
    BlockBase<T,BlockClass>::BlockBase(size_t size) : mSize(size), mInit(size != 0) {}

    template <typename T, class BlockClass>
    size_t BlockBase<T,BlockClass>::size() const
    {
	return mSize;
    }

    template <typename T, class BlockClass>
    bool BlockBase<T,BlockClass>::init() const
    {
	return mInit;
    }
    
    /************************
     *   MutableBlockBase   *
     ************************/
    
    template <typename T, class BlockClass>
    MutableBlockBase<T,BlockClass>::MutableBlockBase(size_t size) : BlockBase<T,BlockClass>(size) {}

    
    /*************
     *   Block   *
     *************/

    template <typename T>
    Block<T>::Block(const T* data,
		    size_t begPos, size_t size, size_t stepSize) :
	BlockBase<T,Block>(size),
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

    
    /******************
     *   BlockArray   *
     ******************/
    
    template <class BlockClass>
    BlockArray<BlockClass>::BlockArray(const BlockClass& block, size_t size, size_t stepSize) :
	BlockBase<BlockClass,BlockArray<BlockClass> >(size),
	mBlock(block), mStepSize(stepSize) {}

    template <class BlockClass>
    BlockType BlockArray<BlockClass>::type() const
    {
	return BlockType::ARRAY;
    }

    template <class BlockClass>
    BlockClass& BlockArray<BlockClass>::operator[](size_t pos)
    {
	return mBlock.set(mStepSize * pos);
    }

    template <class BlockClass>
    BlockArray<BlockClass>& BlockArray<BlockClass>::set(size_t npos)
    {
	mBlock.set(npos);
	return *this;
    }

    template <class BlockClass>
    size_t BlockArray<BlockClass>::stepSize() const
    {
	return mStepSize;
    }
	
    
    /**************
     *   MBlock   *
     **************/

    template <typename T>
    MBlock<T>::MBlock(T* data,
		      size_t begPos, size_t size, size_t stepSize) :
	MutableBlockBase<T,MBlock>(size),
	mData(data),
	mBegPtr(data + begPos),
	mStepSize(stepSize) {}

    template <typename T>
    template <class BlockClass>
    MBlock<T>& MBlock<T>::operator=(const BlockClass& in)
    {
	for(size_t i = 0; i != BB::mSize; ++i){
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
    BlockResult<T>::BlockResult() : MutableBlockBase<T,BlockResult>() {}

    template <typename T>
    BlockResult<T>::BlockResult(const BlockResult<T>& in) : MutableBlockBase<T,BlockResult>(in.size())
    {
	if(BB::mInit){
	    mResPtr = new T[BB::mSize];
	}
	for(size_t i = 0; i != BB::mSize; ++i){
	    mResPtr[i] = in.mResPtr[i];
	}
	mBegPtr = mResPtr;
    }

    template <typename T>
    BlockResult<T>::BlockResult(BlockResult<T>&& in) : MutableBlockBase<T,BlockResult>(in.size())
    {
	if(BB::mInit){
	    mResPtr = in.mResPtr;
	    mBegPtr = mResPtr;
	}
	in.mSize = 0;
	in.mInit = false;
	in.mResPtr = nullptr;
	in.mBegPtr = nullptr;
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
	mBegPtr = mResPtr;
	return *this;
    }

    template <typename T>
    BlockResult<T>& BlockResult<T>::operator=(BlockResult<T>&& in)
    {
	BB::mSize = in.size();
	BB::mInit = BB::mInit and BB::mSize != 0;
	if(BB::mInit){
	    mResPtr = in.mResPtr;
	    mBegPtr = mResPtr;
	}
	in.mSize = 0;
	in.mInit = false;
	in.mResPtr = nullptr;
	return *this;
    }

    template <typename T>
    template <typename... ArgTypes>
    BlockResult<T>::BlockResult(size_t size, const ArgTypes&... args) :
	MutableBlockBase<T,BlockResult>(size)
    {
	if(BB::mInit){
	    mResPtr = new T[BB::mSize](args...);
	}
	for(size_t i = 0; i != BB::mSize; ++i){
	    mResPtr[i] = static_cast<T>( 0 );
	}
	mBegPtr = mResPtr;
    }

    template <typename T>
    BlockResult<T>::~BlockResult()
    {
	delete[] mResPtr;
	mResPtr = nullptr;
	mBegPtr = nullptr;
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
	return mBegPtr[i];
    }

    template <typename T>
    T& BlockResult<T>::operator[](size_t i)
    {
	return mBegPtr[i];
    }

    template <typename T>
    BlockResult<T>& BlockResult<T>::set(size_t npos)
    {
	mBegPtr = mResPtr + npos;
	return *this;
    }

    template <typename T>
    size_t BlockResult<T>::stepSize() const
    {
	return 1;
    }
    
    template <typename T>
    template <typename... Args>
    BlockResult<T>& BlockResult<T>::init(size_t size, const Args&... args)
    {
	BB::mSize = size;
	delete[] mResPtr;
	if(BB::mSize != 0){
	    BB::mInit = true;
	    mResPtr = new T[BB::mSize](args...);
	}
	else {
	    BB::mInit = false;
	}
	for(size_t i = 0; i != BB::mSize; ++i){
	    mResPtr[i] = static_cast<T>( 0 );
	}
	mBegPtr = mResPtr;
	return *this;
    }
    
} // end namespace MultiArrayHelper

#endif
