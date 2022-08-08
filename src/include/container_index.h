// -*- C++ -*-

#ifndef __cxz_container_index_h__
#define __cxz_container_index_h__

#include <cstdlib>
#include <tuple>
#include <memory>

#include "ranges/range_base.h"
#include "ranges/index_base.h"
#include "mbase_def.h"
#include "statics/static_for.h"
#include "ranges/range_helper.h"

namespace CNORXZ
{
    
    template <typename T, class... Indices>
    class ConstContainerIndex : public IndexInterface<ConstContainerIndex<T,Indices...>,
						      std::tuple<typename Indices::MetaType...> >
    {
    public:

	typedef IndexInterface<ConstContainerIndex<T,Indices...>,
			       std::tuple<typename Indices::MetaType...> > IB;
	typedef std::tuple<typename Indices::MetaType...>  MetaType;
	typedef std::tuple<std::shared_ptr<Indices>...> IndexPack;
	typedef ContainerRange<typename Indices::RangeType...> RangeType;

	static constexpr IndexType sType() { return IndexType::CONT; }
	static constexpr size_t sDim() { return sizeof...(Indices); }
	static constexpr size_t totalDim() { return (... * Indices::totalDim()); }

	static constexpr SpaceType STYPE = SpaceType::ANY;
        static constexpr bool PARALLEL = std::tuple_element<0,std::tuple<Indices...>>::type::PARALLEL;
        
	template <typename X>
	using CIX = ConstContainerIndex<X,Indices...>;
	
	template <typename X>
	friend class CIX;
	
    private:
	
	ConstContainerIndex() = default;
	
	bool mNonTrivialBlocks = false;
	bool mExternControl = false;
	IndexPack mIPack;
	std::array<size_t,sizeof...(Indices)+1> mBlockSizes; 
	const T* mData = nullptr;
	std::intptr_t mObjPtrNum;

    protected:
	size_t mCPos;
	
    public:

	ConstContainerIndex(const ConstContainerIndex& in) = default;	
	ConstContainerIndex& operator=(const ConstContainerIndex& in) = default;

	ConstContainerIndex(const ConstContainerIndex& in, bool copy);
	
	ConstContainerIndex& copy(const ConstContainerIndex& in);
	
	template <typename X>
	ConstContainerIndex& operator=(const ConstContainerIndex<X,Indices...>& in);
	
	template <class MRange>
	ConstContainerIndex(const std::shared_ptr<MRange>& range,
		       std::intptr_t objPtrNum);

	template <class MRange>
	ConstContainerIndex(const std::shared_ptr<MRange>& range,
		       std::intptr_t objPtrNum,
		       const std::array<size_t,sizeof...(Indices)+1>& blockSizes);


	template <size_t N>
	size_t getBlockSize() const { return std::get<N>(mBlockSizes); }

	const IndexPack& pack() const { return mIPack; }
	
	ConstContainerIndex& sync(); // recalculate 'IB::mPos' when externalControl == true
	ConstContainerIndex& operator()(const std::shared_ptr<Indices>&... inds); // control via external indices
        ConstContainerIndex& operator()(const std::tuple<std::shared_ptr<Indices>...>& inds);
	ConstContainerIndex& operator()(); // -> sync; just to shorten the code

	// ==== >>>>> STATIC POLYMORPHISM <<<<< ====
	
	IndexType type() const;
	
	ConstContainerIndex& operator++();
	ConstContainerIndex& operator--();
	
	ConstContainerIndex& operator=(size_t pos);

	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);

	std::string stringMeta() const;
	MetaType meta() const;
	ConstContainerIndex& at(const MetaType& metaPos);

	size_t dim() const;
	bool first() const;
	bool last() const;
	bool sliceMode() const;
	
	std::shared_ptr<RangeType> range();
	
	template <size_t N>
	auto& get() const;

	template <size_t N>
	auto getPtr() const;

	size_t getStepSize(size_t n);

	template <class Exprs>
	auto ifor(size_t step, Exprs exs) const;

	template <class Exprs>
	auto iforh(size_t step, Exprs exs) const;

        template <class Exprs>
	auto pifor(size_t step, Exprs exs) const;

	std::intptr_t container() const;
	ConstContainerIndex& format(const std::array<size_t,sizeof...(Indices)+1>& blocks);
	
	// Iterator Stuff
	
	ConstContainerIndex& setData(const T* data);
	
	const T& operator*() const;
	const T* operator->() const;
	//T& operator*();
	//T* operator->();

	ConstContainerIndex operator++(int);
	ConstContainerIndex operator--(int);
	ConstContainerIndex& operator+=(int diff);
	ConstContainerIndex& operator-=(int diff);
	ConstContainerIndex operator+(int num) const;
	ConstContainerIndex operator-(int num) const;

	int operator-(const ConstContainerIndex& it) const;
	const T& operator[](int num) const;

	bool operator<(const ConstContainerIndex& it) const;
	bool operator>(const ConstContainerIndex& it) const;
	bool operator<=(const ConstContainerIndex& it) const;
	bool operator>=(const ConstContainerIndex& it) const;
	
    };
    
    template <typename T, class... Indices>
    class ContainerIndex : public ConstContainerIndex<T,Indices...>
    {
    public:

	typedef ConstContainerIndex<T,Indices...> CCI;
	typedef CCI IB;
	typedef typename CCI::MetaType MetaType;
	typedef typename CCI::IndexPack IndexPack;
	typedef typename CCI::RangeType RangeType;

	static constexpr IndexType sType() { return CCI::sType(); }
	static constexpr size_t sDim() { return CCI::sDim(); }
	static constexpr size_t totalDim() { return CCI::totalDim(); }

	static constexpr SpaceType STYPE = CCI::STYPE;
        static constexpr bool PARALLEL = CCI::PARALLEL;
        
	template <typename X>
	using CIX = ContainerIndex<X,Indices...>;
	
	template <typename X>
	friend class CIX;
	
    private:
	
	ContainerIndex() = default;
	
	T* mMData = nullptr;
	
    public:

	ContainerIndex(const ContainerIndex& in) = default;	
	ContainerIndex& operator=(const ContainerIndex& in) = default;

	ContainerIndex(const ContainerIndex& in, bool copy) : CCI(in,copy)
	{ mMData = in.mMData; }

	ContainerIndex(const ConstContainerIndex<T,Indices...>& in, T* data) : CCI(in)
	{ mMData = data; }

	ContainerIndex(const ConstContainerIndex<T,Indices...>& in, T* data, bool copy) :
	    CCI(in,copy)
	{ mMData = data; }

	ContainerIndex& copy(const ContainerIndex& in)
	{ CCI::copy(in); mMData = in.mMData; }
	
	template <typename X>
	ContainerIndex& operator=(const ContainerIndex<X,Indices...>& in)
	{ CCI::operator=(in); return *this; }
	
	template <class MRange>
	ContainerIndex(const std::shared_ptr<MRange>& range,
		       std::intptr_t objPtrNum) : CCI(range, objPtrNum) {}

	template <class MRange>
	ContainerIndex(const std::shared_ptr<MRange>& range,
		       std::intptr_t objPtrNum,
		       const std::array<size_t,sizeof...(Indices)+1>& blockSizes)
	    : CCI(range, objPtrNum, blockSizes) {}


	template <size_t N>
	size_t getBlockSize() const { return CCI::template getBlockSize<N>(); }

	const IndexPack& pack() const { CCI::pack(); return *this; }
	
	ContainerIndex& sync() { return CCI::sync(); return *this; }
	ContainerIndex& operator()(const std::shared_ptr<Indices>&... inds)
	{ CCI::operator()(inds...); return *this; }
        ContainerIndex& operator()(const std::tuple<std::shared_ptr<Indices>...>& inds)
	{ CCI::operator()(inds); return *this; }
	ContainerIndex& operator()() { CCI::operator()(); return *this; }

	// ==== >>>>> STATIC POLYMORPHISM <<<<< ====
	
	IndexType type() const { return CCI::type(); }
	
	ContainerIndex& operator++() { CCI::operator++(); return *this; }
	ContainerIndex& operator--() { CCI::operator--(); return *this; }
	
	ContainerIndex& operator=(size_t pos) { CCI::operator=(pos); return *this; }

	int pp(std::intptr_t idxPtrNum) { return CCI::pp(idxPtrNum); }
	int mm(std::intptr_t idxPtrNum) { return CCI::mm(idxPtrNum); }

	std::string stringMeta() const { return CCI::stringMeta; }
	MetaType meta() const { return CCI::meta(); }
	ContainerIndex& at(const MetaType& metaPos) { CCI::at(metaPos); return *this; }

	size_t dim() const { return CCI::dim(); }
	bool first() const { return CCI::first(); }
	bool last() const { return CCI::last(); }
	bool sliceMode() const { return CCI::sliceMode(); }
	
	std::shared_ptr<RangeType> range() { return CCI::range(); }
	
	template <size_t N>
	auto& get() const { return CCI::template get<N>(); }

	template <size_t N>
	auto getPtr() const { return CCI::template getPtr<N>(); }

	size_t getStepSize(size_t n) { return getStepSize(n); }

	template <class Exprs>
	auto ifor(size_t step, Exprs exs) const { return CCI::ifor(step, exs); }

	template <class Exprs>
	auto iforh(size_t step, Exprs exs) const { return CCI::iforh(step, exs); }

        template <class Exprs>
	auto pifor(size_t step, Exprs exs) const { return CCI::pifor(step, exs); }

	std::intptr_t container() const { return CCI::container(); }
	ContainerIndex& format(const std::array<size_t,sizeof...(Indices)+1>& blocks)
	{ CCI::format(blocks); return *this; }
	
	// Iterator Stuff
	
	ContainerIndex& setData(T* data) { CCI::setData(data); mMData = data; return *this; }
	
	const T& operator*() const { return CCI::operator*(); }
	const T* operator->() const { return CCI::operator->(); }
	T& operator*() { return mMData[CCI::mCPos]; }
	T* operator->() { return &mMData[CCI::mCPos]; }

	ContainerIndex operator++(int) { auto tmp = *this; ++(*this); return tmp; }
	ContainerIndex operator--(int) { auto tmp = *this; --(*this); return tmp; }
	ContainerIndex& operator+=(int diff) { CCI::operator+=(diff); return *this; }
	ContainerIndex& operator-=(int diff) { CCI::operator-=(diff); return *this; }
	ContainerIndex operator+(int num) const { CCI::operator+(num); return *this; }
	ContainerIndex operator-(int num) const { CCI::operator-(num); return *this; }

	int operator-(const ContainerIndex& it) const { return CCI::operator-(it); }
	const T& operator[](int num) const { return CCI::operator[](num); }

	bool operator<(const ContainerIndex& it) const { return CCI::operator<(it); }
	bool operator>(const ContainerIndex& it) const { return CCI::operator>(it); }
	bool operator<=(const ContainerIndex& it) const { return CCI::operator<=(it); }
	bool operator>=(const ContainerIndex& it) const { return CCI::operator>=(it); }
	
    };

} // end namespace CNORXZ

#endif
