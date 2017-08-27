// -*- C++ -*-

#ifndef __block_h__
#define __block_h__

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
	RESULT = 4,
    };
    
    // manage vectorization in the future !!
    
    template <typename T>
    class BlockBase
    {
    public:
	BlockBase() = default;
	BlockBase(size_t size);

	virtual BlockType type() const = 0;
	
	virtual size_t size() const;
	virtual const T& operator[](size_t pos) const = 0;

	virtual BlockBase& set(const T* nbeg) = 0;
	
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
    class Block : public BlockBase<T>
    {
    public:
	Block() = default;
	Block(const std::vector<T>& data, size_t begPos, size_t size);

	virtual BlockType type() const override;
	virtual const T& operator[](size_t pos) const override;
	virtual Block& set(const T* nbeg) override;
	
    protected:
	const T* mBegPtr;
    };

    template <typename T>
    class BlockValue : public BlockBase<T>
    {
    public:
	BlockValue() = default;
	BlockValue(const T& val, size_t size);

	virtual BlockType type() const override;
	virtual const T& operator[](size_t pos) const override;
	virtual BlockValue& set(const T* nbeg) override;
	
    protected:
	T mVal;
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
	virtual SplitBlock& set(const T* nbeg) override;
	
    protected:
	size_t mStepSize;
	const T* mBegPtr;	
    };

    template <typename T>
    class BlockResult : public BlockBase<T>
    {
    public:
	BlockResult() = default;
	BlockResult(size_t size);

	virtual BlockType type() const override;
	virtual const T& operator[](size_t pos) const override;
	virtual T& operator[](size_t i);
	virtual BlockResult& set(const T* nbeg) override;
	
    protected:
	std::vector<T> mRes;
    };
    
} // end namespace MultiArrayHelper

#include "block.cc"

#endif
