
#ifndef __cxz_dynamic_meta_h__
#define __cxz_dynamic_meta_h__

#include <cstdlib>
#include <utility>
#include <memory>

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

    // NEW:
    // Type Eraser:
    class DType
    {
    public:
	DEFAULT_MEMBERS(DType);

	virtual std::string str() const = 0;
	virtual bool operator==(const DType& in) const = 0;
	virtual bool operator!=(const DType& in) const = 0;
	virtual size_t size() const = 0;
	virtual std::shared_ptr<DType> operator[](size_t pos) const = 0;
    };

    // NEW:
    template <typename T>
    class TypeWrapper : public DType
    {
    private:
	T mD;

    public:
	TypeWrapper(const T& d) : mD(d) {}
	DEFAULT_MEMBERS(TypeWrapper);
	
	virtual std::string str() const { return std::to_string(mD); /*wrapper*/ }
	virtual bool operator==(const DType& in) const { return this->str() == in.str(); }
	virtual bool operator!=(const DType& in) const { return this->str() != in.str(); }
	virtual size_t size() const { return 1; }
	virtual std::shared_ptr<DType> operator[](size_t pos) const { return nullptr; }
    };

    // NEW:
    template <typename T>
    class TypeRefWrapper : public DType
    {
    private:
	const T* mD;

    public:
	TypeRefWrapper(const T* d) : mD(d) {}
	DEFAULT_MEMBERS(TypeRefWrapper);

	virtual std::string str() const { return to_string(*mD); /*wrapper*/ }
	virtual bool operator==(const DType& in) const { return this->str() == in.str(); }
	virtual bool operator!=(const DType& in) const { return this->str() != in.str(); }
	virtual size_t size() const { return 1; }
	virtual std::shared_ptr<DType> operator[](size_t pos) const { return nullptr; }
    };

    // NEW:
    template <typename T>
    class TypeWrapper<std::vector<T>> : public DType
    {
    private:
	std::vector<T> mD;

    public:
	TypeWrapper(const std::vector<T>& d) : mD(d) {}
	DEFAULT_MEMBERS(TypeWrapper);
	
	virtual std::string str() const { return to_string(mD); /* overload before!!! */ }
	virtual bool operator==(const DType& in) const { return this->str() == in.str(); }
	virtual bool operator!=(const DType& in) const { return this->str() != in.str(); }
	virtual size_t size() const { return mD.size(); }
	virtual std::shared_ptr<DType> operator[](size_t pos) const { return std::make_shared<TypeRefWrapper>(&mD[pos]); }
    };

    // NEW:
    template <typename T>
    class TypeRefWrapper<std::vector<T>> : public DType
    {
    private:
	const std::vector<T>* mD;

    public:
	TypeRefWrapper(const std::vector<T>* d) : mD(d) {}
	DEFAULT_MEMBERS(TypeRefWrapper);
	
	virtual std::string str() const { return to_string(*mD); /* overload before!!! */ }
	virtual bool operator==(const DType& in) const { return this->str() == in.str(); }
	virtual bool operator!=(const DType& in) const { return this->str() != in.str(); }
	virtual size_t size() const { return mD->size(); }
	virtual std::shared_ptr<DType> operator[](size_t pos) const { return std::make_shared<TypeRefWrapper>(&(*mD)[pos]); }
    };

    // SPECIALIZE: for std::array<T,N> and std::tuple<T...>
    
} // namespace CNORXZ

#endif
