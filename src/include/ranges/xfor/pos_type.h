
#ifndef __cxz_pos_type_h__
#define __cxz_pos_type_h__

#include "base/base.h"

namespace CNORXZ
{
    // shift to base.h
#define CXZ_CVAL_FALSE static constexpr bool value = false
#define CXZ_CVAL_TRUE static constexpr bool value = true
    
    template <class T>
    struct is_pos_type { CXZ_CVAL_FALSE; };

    template <class T>
    struct is_scalar_pos_type { CXZ_CVAL_FALSE; };
    
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

        template <class PosT>
	constexpr UPos operator*(const PosT& a) const;

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

    template <SizeT N> struct is_pos_type<SPos<N>> { CXZ_CVAL_TRUE; };
    template <SizeT N> struct is_scalar_pos_type<SPos<N>> { CXZ_CVAL_TRUE; };
    template <> struct is_pos_type<UPos> { CXZ_CVAL_TRUE; };
    template <> struct is_scalar_pos_type<UPos> { CXZ_CVAL_TRUE; };
    template <> struct is_pos_type<FPos> { CXZ_CVAL_TRUE; };
    template <> struct is_scalar_pos_type<FPos> { CXZ_CVAL_TRUE; };
    template <SizeT N, SizeT... Ms> struct is_pos_type<SFPos<N,Ms...>> { CXZ_CVAL_TRUE; };
    template <SizeT N, SizeT... Ms> struct is_scalar_pos_type<SFPos<N,Ms...>> { CXZ_CVAL_TRUE; };
    template <class BPosT, class NPosT> struct is_pos_type<MPos<BPosT,NPosT>> { CXZ_CVAL_TRUE; };

} // end namespace CNORXZInternal


#endif
