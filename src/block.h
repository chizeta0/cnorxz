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

    template <typename T, class OpFunc>
    class BlockBinaryOp
    {
    public:
	BlockBinaryOp() = default;
	BlockResult<T> operator()(const BlockBase<T>& arg1, const BlockBase<T>& arg2);
    };
    
    template <typename T>
    class BlockBase
    {
    public:
	BlockBase() = default;
	BlockBase(size_t size);

	BlockBase(BlockBase&& res) = default;
	BlockBase& operator=(BlockBase&& res) = default;
	
	virtual BlockType type() const = 0;
	virtual size_t stepSize() const = 0;
	
	virtual size_t size() const;
	virtual const T& operator[](size_t pos) const = 0;

	virtual BlockBase& set(size_t npos) = 0;
	
	template <class OpFunction>
	BlockResult<T> operate(const BlockBase& in);
	
	BlockResult<T> operator+(const BlockBase& in);
	BlockResult<T> operator-(const BlockBase& in);
	BlockResult<T> operator*(const BlockBase& in);
	BlockResult<T> operator/(const BlockBase& in);
	
    protected:
	size_t mSize;
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& out, const BlockBase<T>& block)
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

	MutableBlockBase() = default;
	MutableBlockBase(size_t size);

	MutableBlockBase& operator=(const BlockBase<T>& in);
	
	MutableBlockBase(MutableBlockBase&& res) = default;
	MutableBlockBase& operator=(MutableBlockBase&& res) = default;

	virtual T& operator[](size_t pos) = 0;
	
    };
    
    template <typename T>
    class Block : public BlockBase<T>
    {
    public:
	Block() = default;
	Block(const std::vector<T>& data, size_t begPos, size_t size);

	virtual BlockType type() const override;
	virtual const T& operator[](size_t pos) const override;
	virtual Block& set(size_t npos) override;
	virtual size_t stepSize() const override;
	
    protected:
	const std::vector<T>* mData;
	const T* mBegPtr;
    };

    template <typename T>
    class MBlock : public MutableBlockBase<T>
    {
    public:
	MBlock() = default;
	MBlock(std::vector<T>& data, size_t begPos, size_t size);

	virtual BlockType type() const override;
	virtual const T& operator[](size_t pos) const override;
	virtual T& operator[](size_t pos) override;
	virtual MBlock& set(size_t npos) override;
	virtual size_t stepSize() const override;
	
    protected:
	std::vector<T>* mData;
	T* mBegPtr;
    };    
    
    template <typename T>
    class BlockValue : public BlockBase<T>
    {
    public:
	BlockValue() = default;
	BlockValue(const std::vector<T>& data,
		   size_t pos, size_t size);

	virtual BlockType type() const override;
	virtual const T& operator[](size_t pos) const override;
	virtual BlockValue& set(size_t npos) override;
	virtual size_t stepSize() const override;
	
    protected:
	const std::vector<T>* mData;
	const T* mVal;
    };

    template <typename T>
    class MBlockValue : public MutableBlockBase<T>
    {
    public:
	MBlockValue() = default;
	MBlockValue(std::vector<T>& data,
		    size_t pos, size_t size);

	virtual BlockType type() const override;
	virtual const T& operator[](size_t pos) const override;
	virtual T& operator[](size_t pos) override;
	virtual MBlockValue& set(size_t npos) override;
	virtual size_t stepSize() const override;
	
    protected:
	std::vector<T>* mData;
	T* mVal;
    };
    
    template <typename T>
    class SplitBlock : public BlockBase<T>
    {
    public:

	SplitBlock() = default;
	SplitBlock(const std::vector<T>& data, size_t begPos,
		   size_t stepSize, size_t size);

	virtual BlockType type() const override;
	virtual const T& operator[](size_t pos) const override;
	virtual SplitBlock& set(size_t npos) override;
	virtual size_t stepSize() const override;
	
    protected:
	const std::vector<T>* mData;
	size_t mStepSize;
	const T* mBegPtr;	
    };

    template <typename T>
    class MSplitBlock : public MutableBlockBase<T>
    {
    public:

	MSplitBlock() = default;
	MSplitBlock(std::vector<T>& data, size_t begPos,
		    size_t stepSize, size_t size);

	virtual BlockType type() const override;
	virtual const T& operator[](size_t pos) const override;
	virtual T& operator[](size_t pos) override;
	virtual MSplitBlock& set(size_t npos) override;
	virtual size_t stepSize() const override;
	
    protected:
	std::vector<T>* mData;
	size_t mStepSize;
	T* mBegPtr;	
    };

    
    template <typename T>
    class BlockResult : public MutableBlockBase<T>
    {
    public:
	BlockResult() = default;
	BlockResult(size_t size);

	BlockResult(BlockResult&& res) = default;
	BlockResult& operator=(BlockResult&& res) = default;
	
	virtual BlockType type() const override;
	virtual const T& operator[](size_t pos) const override;
	virtual T& operator[](size_t i) override;
	virtual BlockResult& set(size_t npos) override;
	virtual size_t stepSize() const override;
	
    protected:
	std::vector<T> mRes;
    };
    
} // end namespace MultiArrayHelper

#include "block.cc"

#endif
