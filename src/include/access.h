
#ifndef __cxz_access_h__
#define __cxz_access_h__

namespace CNORXZ
{
    namespace
    {
	using namespace CNORXZInternal;
    }

    
    template <class T>
    struct VType
    {
	typedef T type;
	static constexpr size_t MULT = sizeof(type)/sizeof(T);
    };

    template <>
    struct VType<double>
    {
	typedef v256 type;
	static constexpr size_t MULT = sizeof(type)/sizeof(double);
    };

    template <template <typename...> class F,typename... Ts>
    inline auto mkVFuncPtr(const std::shared_ptr<F<Ts...>>& f)
    {
	return std::shared_ptr<F<typename VType<Ts>::type...>>();
	// empty, implement corresponding constructors...!!!
    }

    template <template <typename...> class F,typename... Ts>
    inline auto mkVFunc(const F<Ts...>& f)
    {
	return F<typename VType<Ts>::type...>();
	// empty, implement corresponding constructors...!!!
    }

    
    template <class F>
    using VFunc = decltype(mkVFunc(std::declval<F>()));
    
    //template <class F>
    //using VFunc = F;
    
    template <typename T, class F, class... Indices>
    class OpAccess
    {
    private:
	std::tuple<std::shared_ptr<Indices>...> mInds;
	
    public:
	static constexpr bool ISSTATIC = false;
	
	template <typename Op, class ExtType>
	inline void operator()(T*& t, size_t pos, const Op& op, ExtType e)
	{
	    F::selfApply(t[pos](mInds), op.get(e)); // s.th. like that
	    // TODO for classes related to the r.h.s.:
	    // forward get(e) to elements returned by get(e) until basic types are reached
	    // (also same for rootSteps etc...) !!!!
	    // introduce traits !!!!
	    // !!!!
	}
    };

    // static polymorphism
    template <class AccessClass>
    class AccessTemplate
    {
    public:
        typedef AccessClass AC;

	AccessTemplate(const AccessTemplate& in) = default;
	AccessTemplate(AccessTemplate&& in) = default;
	AccessTemplate& operator=(const AccessTemplate& in) = default;
	AccessTemplate& operator=(AccessTemplate&& in) = default;

        AccessClass& THIS() { return static_cast<AccessClass&>(*this); }
        const AccessClass& THIS() const { return static_cast<const AccessClass&>(*this); }

        inline auto get(size_t pos);
        inline auto get(size_t pos) const;
        inline auto oget(size_t pos) const;

	template <class F, typename Op, class ExtType>
	inline void exec(size_t pos, const Op& op, ExtType e) const;

    protected:
	AccessTemplate() = default;

    };

    template <typename T>
    class PointerAccess : public AccessTemplate<PointerAccess<T>>
    {
    public:
	typedef T value_type;
	typedef T in_type;

	static constexpr size_t VSIZE = sizeof(value_type) / sizeof(in_type);
	
	friend class AccessTemplate<PointerAccess<T>>;
    private:
	PointerAccess() = default;
	
        T* mPtr = nullptr;
        T* mOrigPtr = nullptr;
        
    public:
        PointerAccess(T* ptr, T* origPtr) : mPtr(ptr), mOrigPtr(origPtr) {}
	
	PointerAccess(const PointerAccess& in) = default;
	PointerAccess(PointerAccess&& in) = default;
	PointerAccess& operator=(const PointerAccess& in) = default;
	PointerAccess& operator=(PointerAccess&& in) = default;

        T* get(size_t pos) { return mPtr+pos; }
        T* get(size_t pos) const { return mPtr+pos; }
        PointerAccess<T>& set(size_t pos) { mPtr = mOrigPtr + pos; return *this; }
        T* oget(size_t pos) const { return mOrigPtr+pos; }

	template <class F, typename Op, class ExtType>
	inline void exec(size_t pos, const Op& op, ExtType e) const
        {
	    F::selfApply(*get(pos),op.get(e));
        }
    };

    template <typename T>
    class VPointerAccess : public AccessTemplate<VPointerAccess<T>>
    {
    public:
	typedef typename VType<T>::type value_type;
	typedef T in_type;

	static constexpr size_t VSIZE = sizeof(value_type) / sizeof(in_type);

	friend class AccessTemplate<VPointerAccess<T>>;
    private:
	VPointerAccess() = default;
	
        T* mPtr = nullptr;
        T* mOrigPtr = nullptr;
        
    public:
        VPointerAccess(T* ptr, T* origPtr) : mPtr(ptr), mOrigPtr(origPtr) {}

	VPointerAccess(const PointerAccess<T>& in) : mPtr(in.get(0)), mOrigPtr(in.oget(0)) {} 
	
	VPointerAccess(const VPointerAccess& in) = default;
	VPointerAccess(VPointerAccess&& in) = default;
	VPointerAccess& operator=(const VPointerAccess& in) = default;
	VPointerAccess& operator=(VPointerAccess&& in) = default;

        value_type* get(size_t pos) { return reinterpret_cast<value_type*>(mPtr+pos); }
        value_type* get(size_t pos) const { return reinterpret_cast<value_type*>(mPtr+pos); }
        VPointerAccess<T>& set(size_t pos) { mPtr = mOrigPtr + pos; return *this; }
        value_type* oget(size_t pos) const { return reinterpret_cast<value_type*>(mOrigPtr+pos); }

	template <class F, typename Op, class ExtType>
	inline void exec(size_t pos, const Op& op, ExtType e) const
        {
	    F::selfApply(*get(pos),op.template vget<value_type>(e));
        }
    };

    
} // namespace CNORXZ

#endif
