
#ifndef __cxz_aindex_h__
#define __cxz_aindex_h__

#include "ranges/range_base.h"
#include "ranges/index_base.h"
#include "ranges/xindex.h"
#include "ranges/yrange.h"

namespace CNORXZ
{

    // AIndex (A = Array)
    template <typename T>
    class AIndex : public YIndex
    {
    public:
	typedef YIndex::IB IB;
	using YIndex::operator=;

	DEFAULT_MEMBERS(AIndex);
	AIndex(const T* data, const RangePtr& range, SizeT lexpos = 0);
	AIndex(const T* data, const YIndex& yindex);
	
	AIndex operator+(Int n) const;
	AIndex operator-(Int n) const;

	const T& operator*() const;
	const T* operator->() const;

    protected:
	const T* mCData = nullptr;

    };

    // BIndex (because B comes after A...)
    template <typename T>
    class BIndex : public AIndex<T>
    {
    public:
	typedef AIndex<T> AI;
	typedef typename AI::IB IB;
	
	DEFAULT_MEMBERS(BIndex);
	BIndex(T* data, const RangePtr& range, SizeT pos = 0);
	BIndex(T* data, const AIndex<T>& cci);

	BIndex operator+(Int n) const;
	BIndex operator-(Int n) const;

	T& operator*();
	T* operator->();

    private:
	T* mData = nullptr;

    };
}

#endif
