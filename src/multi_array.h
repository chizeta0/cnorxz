// -*- C++ -*-

#ifndef __multi_array_h__
#define __multi_array_h__

#include <cstdlib>
#include <vector>
#include <memory>

#include "base_def.h"
#include "multi_array_operation.h"
#include "manipulator.h"
#include "name.h"

namespace MultiArrayTools
{

    template <typename T, class Range>
    class MultiArrayBase
    {
    public:

	// iterator ( containing idx of Range )
	
	DEFAULT_MEMBERS(MultiArrayBase);
	MultiArrayBase(const Range& range);

	virtual ~MultiArrayBase() = default;
	
	virtual T& operator[](const typename Range::IndexType& i) = 0;
	virtual const T& operator[](const typename Range::IndexType& i) const = 0;

	virtual size_t size() const; 
	virtual bool isSlice() const = 0;
	
	virtual auto begin() const -> decltype(Range().begin());
	virtual auto end() const -> decltype(Range().end());

	virtual const Range& range() const;

	template <class... NameTypes>
	MultiArrayOperationRoot<T,Range> operator()(const NameTypes&... str);

	template <class NameType>
	MultiArrayOperationRoot<T,Range> operator()(const NameType& name, bool master);

	template <class... NameTypes>
	ConstMultiArrayOperationRoot<T,Range> operator()(const NameTypes&... str) const;

	template <class NameType>
	ConstMultiArrayOperationRoot<T,Range> operator()(const NameType& name, bool master) const;

	virtual bool isInit() const;
	
    protected:
	bool mInit = false;
	std::shared_ptr<Range> mRange;

    };
    
    template <typename T, class Range>
    class MultiArray : public MultiArrayBase<T,Range>
    {
    public:

	typedef MultiArrayBase<T,Range> MAB;

	DEFAULT_MEMBERS(MultiArray);
	MultiArray(const Range& range);
	MultiArray(const Range& range, const std::vector<T>& vec);
	MultiArray(const Range& range, std::vector<T>&& vec);

	template <class Range2, class Range3>
	MultiArray(const MultiArray<MultiArray<T,Range2>,Range3> in);

	template <class Range2, class Range3>
	MultiArray& operator=(const MultiArray<MultiArray<T,Range2>,Range3> in);
	
	T& operator[](const typename Range::IndexType& i) override;
	const T& operator[](const typename Range::IndexType& i) const override;
	
	virtual bool isSlice() const override;

	//	virtual void manipulate(ManipulatorBase<T>& mb,
	//			const typename Range::IndexType& manBegin,
	//				const typename Range::IndexType& manEnd);
	
	template <typename U, class RangeX>
	friend class MultiArray;
	
    private:
	std::vector<T> mCont;
    };

}

#include "multi_array.cc"

#endif
