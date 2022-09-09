
#ifndef __cxz_dynamic_meta_h__
#define __cxz_dynamic_meta_h__

#include <cstdlib>
#include <utility>
#include <memory>
#include <any>
#include <functional>

#include "allocator.h"
#include "rbase_def.h"

namespace CNORXZ
{
    // DEP:
    typedef std::pair<const char*,size_t> DynamicMetaElem;

    template <size_t N>
    struct DynamicMetaSetter
    {
	template <typename... Us>
	static inline void set(const std::tuple<Us...>& in, vector<DynamicMetaElem>& meta)
	{
	    typedef typename std::tuple_element<N,std::tuple<Us...>>::type elemtype;
	    meta[N].first = reinterpret_cast<const char*>( &std::get<N>(in) );
	    meta[N].second = sizeof(elemtype);
	    DynamicMetaSetter<N-1>::set(in,meta);
	}	    
    };

    // DEP:
    template <>
    struct DynamicMetaSetter<0>
    {
	template <typename... Us>
	static inline void set(const std::tuple<Us...>& in, vector<DynamicMetaElem>& meta)
	{
	    typedef typename std::tuple_element<0,std::tuple<Us...>>::type elemtype;
	    meta[0].first = reinterpret_cast<const char*>( &std::get<0>(in) );
	    meta[0].second = sizeof(elemtype);
	}	    
    };

    
    // DEP:
    class DynamicMetaT
    {
    private:
        vector<DynamicMetaElem> mMeta;

    public:
        DynamicMetaT() = default;
        DynamicMetaT(const DynamicMetaT& in) = default;
        DynamicMetaT(DynamicMetaT&& in) = default;
        DynamicMetaT& operator=(const DynamicMetaT& in) = default;
        DynamicMetaT& operator=(DynamicMetaT&& in) = default;

        template <typename... Us>
        DynamicMetaT(const std::tuple<Us...>* meta) : mMeta(sizeof...(Us))
        {
	    DynamicMetaSetter<sizeof...(Us)-1>::set(*meta,mMeta);
        }
        
        bool operator==(const DynamicMetaT& in) const;
        bool operator!=(const DynamicMetaT& in) const;

        size_t size() const;
        
        DynamicMetaElem& operator[](size_t pos);
        const DynamicMetaElem& operator[](size_t pos) const;
    };

    class DType
    {
    private:
	std::any mD;
	std::function<std::string()> mToStr;
	std::function<int(std::any)> mComp;

	template <typename T>
	void _mkToStr();

	template <typename T>
	void _mkComp();
	
    public:
	DEFAULT_MEMBERS(DType);

	template <typename T>
	DType(const T& d);
	
	template <typename T>
	DType& operator=(const T& d);

	std::string str() const { return mToStr(); }
	const std::any& get() const { return mD; }

	bool operator==(const DType& a) const { return mComp(a.mD) == 0; }
	bool operator!=(const DType& a) const { return mComp(a.mD) != 0; }
	bool operator<(const DType& a) const { return mComp(a.mD) == -1; }
	bool operator>(const DType& a) const { return mComp(a.mD) == 1; }
	bool operator<=(const DType& a) const { auto c = mComp(a.mD); return c <= 0; }
	bool operator>=(const DType& a) const { auto c = mComp(a.mD); return c == 1 or c == 0; }
    };
    
} // namespace CNORXZ

#endif
