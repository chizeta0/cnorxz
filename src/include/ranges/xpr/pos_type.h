
#ifndef __cxz_pos_type_h__
#define __cxz_pos_type_h__

#include "base/base.h"

namespace CNORXZ
{
    
    template <SizeT N>
    class SPos
    {
    public:
	constexpr SPos() = default;
	constexpr SPos(const SPos& a) = default;
	constexpr SPos(SPos&& a) = default;

	constexpr SizeT size() const;
	constexpr SizeT val() const;
	
	template <SizeT N1>
	constexpr auto operator+(const SPos<N1>& a) const;
	template <SizeT N1>
	constexpr auto operator*(const SPos<N1>& a) const;
	template <SizeT N1>
	constexpr auto operator()(const SPos<N1>& a) const;

	constexpr auto operator+(const UPos& a) const;
	constexpr auto operator*(const UPos& a) const;
	constexpr auto operator()(const UPos& a) const;
	
	template <class PosT>
	constexpr auto extend(const PosT& a) const;

	explicit constexpr operator UPos() const;
    };

    class UPos
    {
    private:
        SizeT mExt = 0;
    public:
	DEFAULT_MEMBERS(UPos);

	constexpr UPos(SizeT ext);
	
	constexpr SizeT size() const;
	constexpr const SizeT& val() const;

	template <class PosT>
	constexpr UPos operator+(const PosT& a) const;

	constexpr SPos<0> operator*(const SPos<0>& a) const;

        template <class PosT>
	constexpr UPos operator*(const PosT& a) const;

	constexpr SPos<0> operator()(const SPos<0>& a) const;

	template <class PosT>
	constexpr UPos operator()(const PosT& a) const;

        template <class PosT>
        constexpr auto extend(const PosT& a) const;

    };

    class FPos
    {
    private:
	SizeT mExt = 0;
	const SizeT* mMap = nullptr;

    public:
	DEFAULT_MEMBERS(FPos);

	inline FPos(SizeT ext, const SizeT* map);

	constexpr SizeT size() const;
	constexpr const SizeT& val() const;
	
	template <class PosT1>
	constexpr UPos operator+(const PosT1& a) const;

	template <class PosT1>
	constexpr FPos operator*(const PosT1& a) const;

	template <class PosT>
	constexpr UPos operator()(const PosT& a) const;

	template <class PosT1>
	constexpr auto extend(const PosT1& a) const;

    };

    template <SizeT N, SizeT... Ms>
    class SFPos
    {
    private:
	static Arr<SizeT,sizeof...(Ms)> sMs;
    public:
	constexpr SFPos() = default;

	constexpr SizeT size() const;
	constexpr SizeT val() const;

	template <SizeT N1>
	constexpr auto operator+(const SPos<N1>& a) const;
	template <SizeT N1>
	constexpr auto operator*(const SPos<N1>& a) const;
	template <SizeT N1>
	constexpr auto operator()(const SPos<N1>& a) const;

	constexpr auto operator+(const UPos& a) const;
	constexpr auto operator*(const UPos& a) const;
	constexpr auto operator()(const UPos& a) const;

	template <class PosT>
	constexpr auto extend(const PosT& a) const;

	explicit constexpr operator FPos() const;
    };

    template <class BPosT, class NPosT>
    class MPos : public BPosT // BPos should be a SCALAR PosT (NO MPos!)
    {
    protected:
	NPosT mNext;
    public:
	constexpr MPos();

	constexpr MPos(const BPosT& b, const NPosT& n);
	
	template <typename... Args>
	constexpr MPos(Args&&... args, const NPosT& next);

	template <typename... Args>
	constexpr MPos(Args&&... args, NPosT&& next);

	constexpr SizeT size() const;
	constexpr const NPosT& next() const;

	template <class PosT>
	constexpr auto operator+(const PosT& a) const;

	template <class PosT>
	constexpr auto operator*(const PosT& a) const;

	// same as operator*, except for FPos/SFPos, where map is executed
	template <class PosT>
	constexpr auto operator()(const PosT& a) const;
	
	template <class PosT>
	constexpr auto extend(const PosT& a) const;
    };

    class DPos : public ObjHandle<VPosBase>
    {
    public:
	DEFAULT_MEMBERS(DPos);

	inline DPos(Uptr<VPosBase>&& a);
	inline DPos(const DPosRef& a);
	
	template <class PosT>
	inline explicit DPos(const PosT& a);

	template <class PosT>
	inline explicit DPos(PosT&& a);

	inline const VPosBase* get() const;
	inline const VPosBase* vpos() const;
	inline SizeT size() const;
	inline SizeT val() const;
	inline DPosRef next() const;

	template <class PosT>
	inline DPos operator+(const PosT& a) const;

	template <class PosT>
	inline DPos operator*(const PosT& a) const;

	// same as operator*, except for FPos/SFPos, where map is executed
	template <class PosT>
	inline DPos operator()(const PosT& a) const;
	
	template <class PosT>
	inline DPos extend(const PosT& a) const;
    };

    class DPosRef
    {
    private:
	const VPosBase* mP = nullptr;

    public:
	DEFAULT_MEMBERS(DPosRef);

	explicit DPosRef(const VPosBase* p);

	inline const VPosBase* get() const;
	inline const VPosBase* vpos() const;
	inline SizeT size() const;
	inline SizeT val() const;
	inline DPosRef next() const;

	template <class PosT>
	inline DPos operator+(const PosT& a) const;

	template <class PosT>
	inline DPos operator*(const PosT& a) const;

	// same as operator*, except for FPos/SFPos, where map is executed
	template <class PosT>
	inline DPos operator()(const PosT& a) const;
	
	template <class PosT>
	inline DPos extend(const PosT& a) const;
    };

    // for common call of extension vector elements
    // BPos = base pos type
    // OPos = offset pos type (i.e. step/ext * for index of lowest for layer)
    // currently EPos must be used only at the lowest for layer
    template <class BPosT, class... OPosTs>
    class EPos : public BPosT
    {
    private:
	Tuple<OPosTs...> mP;

	template <SizeT... Is>
	constexpr decltype(auto) ival(std::index_sequence<Is...> is) const;

	template <SizeT... Is>
	constexpr decltype(auto) inext(std::index_sequence<Is...> is) const;
	
    public:
	DEFAULT_MEMBERS(EPos);

	constexpr EPos(const BPosT& b, const OPosTs&... os);
	constexpr EPos(BPosT&& b, OPosTs&&... os);

	constexpr decltype(auto) val() const;
	constexpr decltype(auto) next() const;
    };

    /*********************************
     *   Traits and Helper-Classes   *
     *********************************/
    
    template <class T>
    struct is_pos_type { CXZ_CVAL_FALSE; };

    template <class T>
    struct is_scalar_pos_type { CXZ_CVAL_FALSE; };

    template <class T>
    struct is_static_pos_type { CXZ_CVAL_FALSE; };

    template <class T>
    struct is_epos_type { CXZ_CVAL_FALSE; };
    
    template <class PosT>
    struct pos_depth
    {
	static constexpr SizeT value = is_pos_type<PosT>::value ? 1 : 0;
    };
    
    template <class OPosT1, class OPosT2, class... OPosTs>
    struct pos_type_consecutive_2
    {
    private:
	static constexpr bool eval();
	
    public:
	static constexpr bool value = eval();
    };

    template <class OPosT1, class... OPosTs>
    struct pos_type_consecutive
    {
	static constexpr bool value = sizeof...(OPosTs) == 0 ? is_pos_type<OPosT1>::value : pos_type_consecutive_2<OPosT1,OPosTs...>::value;
    };

    template <class OPosT1, class... OPosTs>
    struct pos_type_same
    {
    private:
	static constexpr bool eval();
	
    public:
	static constexpr bool value = eval();
    };
    
    template <class PosT> struct pos_type_is_consecutive { CXZ_CVAL_FALSE; };

    template <SizeT N, class BPosT, class OPosT>
    decltype(auto) mkEPos(const BPosT& a, const OPosT& b);

    template <SizeT N, class BPosT, class OPosT, SizeT... Is>
    decltype(auto) mkiEPos(const BPosT& a, const OPosT& b, std::index_sequence<Is...> is);

    /**************************************************
     *   Traits and Helper-Classes: Specializations   *
     **************************************************/

    template <SizeT N> struct is_pos_type<SPos<N>> { CXZ_CVAL_TRUE; };
    template <SizeT N> struct is_scalar_pos_type<SPos<N>> { CXZ_CVAL_TRUE; };
    template <SizeT N> struct is_static_pos_type<SPos<N>> { CXZ_CVAL_TRUE; };

    template <> struct is_pos_type<UPos> { CXZ_CVAL_TRUE; };
    template <> struct is_scalar_pos_type<UPos> { CXZ_CVAL_TRUE; };

    template <> struct is_pos_type<FPos> { CXZ_CVAL_TRUE; };
    template <> struct is_scalar_pos_type<FPos> { CXZ_CVAL_TRUE; };

    template <SizeT N, SizeT... Ms> struct is_pos_type<SFPos<N,Ms...>> { CXZ_CVAL_TRUE; };
    template <SizeT N, SizeT... Ms> struct is_scalar_pos_type<SFPos<N,Ms...>> { CXZ_CVAL_TRUE; };
    template <SizeT N, SizeT... Ms> struct is_static_pos_type<SFPos<N,Ms...>> { CXZ_CVAL_TRUE; };

    template <class BPosT, class NPosT> struct is_pos_type<MPos<BPosT,NPosT>> { CXZ_CVAL_TRUE; };

    template <> struct is_pos_type<DPos> { CXZ_CVAL_TRUE; };
    template <> struct is_pos_type<DPosRef> { CXZ_CVAL_TRUE; };

    template <class BPosT, class... OPosTs> struct is_pos_type<EPos<BPosT,OPosTs...>> { CXZ_CVAL_TRUE; };
    template <class BPosT, class... OPosTs> struct is_scalar_pos_type<EPos<BPosT,OPosTs...>>
    { static constexpr bool value = is_scalar_pos_type<BPosT>::value; };
    template <class BPosT, class... OPosTs> struct is_static_pos_type<EPos<BPosT,OPosTs...>>
    { static constexpr bool value = is_static_pos_type<BPosT>::value and (is_static_pos_type<OPosTs>::value and ...); };
    template <class BPosT, class... OPosTs> struct is_epos_type<EPos<BPosT,OPosTs...>> { CXZ_CVAL_TRUE; };
    
    template <class BPosT, class NPosT>
    struct pos_depth<MPos<BPosT,NPosT>>
    {
	static constexpr SizeT value = pos_depth<NPosT>::value + 1;
    };

    template <class BPosT, class... OPosTs>
    struct pos_type_is_consecutive<EPos<BPosT,OPosTs...>>
    {
	static constexpr bool value = pos_type_is_consecutive<OPosTs...>::value;
    };

} // end namespace CNORXZInternal


#endif
