
#ifndef __dynamic_meta_h__
#define __dynamic_meta_h__

#include <cstdlib>
#include <utility>

namespace MultiArrayTools
{

    typedef std::pair<const char*,size_t> DynamicMetaElem;

    template <size_t N>
    struct DynamicMetaSetter
    {
	template <typename... Us>
	static inline void set(const std::tuple<Us...>& in, std::vector<DynamicMetaElem>& meta)
	{
	    typedef typename std::tuple_element<N,std::tuple<Us...>>::type elemtype;
	    meta[N].first = reinterpret_cast<const char*>( &std::get<N>(in) );
	    meta[N].second = sizeof(elemtype);
	    DynamicMetaSetter<N-1>::set(in,meta);
	}	    
    };

    template <>
    struct DynamicMetaSetter<0>
    {
	template <typename... Us>
	static inline void set(const std::tuple<Us...>& in, std::vector<DynamicMetaElem>& meta)
	{
	    typedef typename std::tuple_element<0,std::tuple<Us...>>::type elemtype;
	    meta[0].first = reinterpret_cast<const char*>( &std::get<0>(in) );
	    meta[0].second = sizeof(elemtype);
	}	    
    };

    
    class DynamicMetaT
    {
    private:
        std::vector<DynamicMetaElem> mMeta;

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


} // namespace MultiArrayTools

#endif
