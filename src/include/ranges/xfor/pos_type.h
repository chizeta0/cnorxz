
#ifndef __cxz_pos_type_h__
#define __cxz_pos_type_h__

#include "base/base.h"

namespace CNORXZ
{

    template <class PosT>
    class CPosInterface
    {
    public:
	static constexpr bool MULTI = PosT::MULTI;
	
	DEFAULT_MEMBERS(CPosInterface);

	PosT& THIS() { return static_cast<PosT&>(*this); }
	const PosT& THIS() const { return static_cast<const PosT&>(*this); }
	
	inline SizeT size() const { return THIS().size(); }
	inline auto val() const { return THIS().val(); }
	// template!!!
	inline CPosInterface<PosT> operator+(const CPosInterface<PosT>& a) const
	{ return THIS() + a.THIS(); }
	// template!!!
	inline CPosInterface<PosT> operator*(const CPosInterface<PosT>& a) const
	{ return THIS() * a.THIS(); }
	// template!!!
	// inline CPosInterface<PosT> execute(const CPosInterface<PosT>& a,
	// const CPosInterface<PosT>& b, const CPosInterface<PosT>& c) const
	// => a+b*c; only this executes also the FPos/SFPos-Map!!! 
	
	// for each class implement +/* for each argument type EXPLICITLY (NO templates, except for MPos)
	// *: only UPos/SPos as Arguments, for DPos only UPos as Args
	
	template <class P>
	inline auto extend(const CPosInterface<P>& a) const { return THIS().extend(a); }
    };

    template <SizeT N>
    class SPos : public CPosInterface<SPos<N>>
    {
    public:
	static constexpr bool MULTI = false;

	constexpr SPos() = default;

	constexpr SizeT size() const;
	constexpr SizeT val() const;

	template <SizeT N1>
	constexpr auto operator+(const SPos<N1>& a) const;
	template <SizeT N1>
	constexpr auto operator*(const SPos<N1>& a) const;

	constexpr auto operator+(const UPos& a) const;
	constexpr auto operator*(const UPos& a) const;

	template <class PosT>
	constexpr auto extend(const CPosInterface<PosT>& a) const;
    };
    
    class UPos : public CPosInterface<UPos>
    {
    private:
        SizeT mExt = 0;
    public:
	static constexpr bool MULTI = false;
	
	DEFAULT_MEMBERS(UPos);

	constexpr UPos(SizeT ext);
	
	constexpr SizeT size() const;
	constexpr const SizeT& val() const;

	template <class PosT>
	constexpr UPos operator+(const CPosInterface<PosT>& a) const;

        template <class PosT>
	constexpr UPos operator*(const CPosInterface<PosT>& a) const;
	
        template <class PosT>
        constexpr auto extend(const CPosInterface<PosT>& y) const;

    };
    
    class FPos : public CPosInterface<FPos>
    {
    private:
	SizeT mExt = 0;
	const SizeT* mMap = nullptr;

    public:
	static constexpr bool MULTI = false;

	DEFAULT_MEMBERS(FPos);

	inline FPos(SizeT ext, const SizeT* map);

	constexpr SizeT size() const;
	constexpr const SizeT& val() const;
	
	template <class PosT1>
	constexpr UPos operator+(const CPosInterface<PosT1>& a) const;

	template <class PosT1>
	constexpr UPos operator*(const CPosInterface<PosT1>& a) const;

	template <class PosT1>
	constexpr auto extend(const CPosInterface<PosT1>& a) const;
    };

    template <SizeT N, SizeT... Ms>
    class SFPos : public CPosInterface<SFPos<N,Ms...>>
    {
    public:
	static constexpr bool MULTI = false;

	constexpr SFPos() = default;

	constexpr SizeT size() const;
	constexpr SizeT val() const;

	template <SizeT N1>
	constexpr auto operator+(const SPos<N1>& a) const;
	template <SizeT N1>
	constexpr auto operator*(const SPos<N1>& a) const;

	constexpr auto operator+(const UPos& a) const;
	constexpr auto operator*(const UPos& a) const;

	template <class PosT>
	constexpr auto extend(const CPosInterface<PosT>& a) const;
    };
    
    class DPos : public ObjHandle<VPosBase>,
		 public CPosInterface<DPos>
    {
    public:
	static constexpr bool MULTI = true;

	DEFAULT_MEMBERS(DPos);
	inline DPos(Uptr<VPosBase>&& a);
	
	template <class PosT>
	inline DPos(const CPosInterface<PosT>& a);

	inline SizeT size() const;
	inline SizeT val() const;
	inline DPosRef first() const;
	inline DPosRef next() const;

	template <class PosT1>
	inline DPos operator+(const CPosInterface<PosT1>& a) const;

	template <class PosT1>
	inline DPos operator*(const CPosInterface<PosT1>& a) const;

	template <class PosT1>
	inline DPos extend(const CPosInterface<PosT1>& a) const;
	
    };
    
    class DPosRef : public CPosInterface<DPosRef>
    {
    private:
	const VPosBase* mC;
    public:
	static constexpr bool MULTI = true;

	DEFAULT_MEMBERS(DPosRef);
	inline DPosRef(const VPosBase* c);

	inline SizeT size() const;
	inline SizeT val() const;
	inline DPosRef first() const;
	inline DPosRef next() const;

	template <class PosT1>
	inline DPos operator+(const CPosInterface<PosT1>& a) const;

	template <class PosT1>
	inline DPos operator*(const CPosInterface<PosT1>& a) const;

	template <class PosT1>
	inline DPos extend(const CPosInterface<PosT1>& a) const;
    };

    // go to original pattern (-> LINEAR template chain)
    // first: just cast by constructor
    template <class PosT1, class PosT2>
    class MPos : public CPosInterface<MPos<PosT1,PosT2>>
    {
    private:

	PosT1 mFirst;
	PosT2 mNext;
	
    public:
	static constexpr bool MULTI = true;

	DEFAULT_MEMBERS(MPos);
	
	constexpr MPos(const CPosInterface<PosT1>& first,
		       const CPosInterface<PosT2>& next);

	constexpr SizeT size() const;
	constexpr auto val() const;
	constexpr const PosT1& first() const;
	constexpr const PosT2& next() const;

	template <class PosT3, class PosT4>
	constexpr auto operator+(const MPos<PosT3,PosT4>& a) const;

	template <class PosT3>
	constexpr auto operator*(const CPosInterface<PosT3>& a) const;

	template <class PosT3>
	constexpr auto extend(const CPosInterface<PosT3>& p) const;

    };    
    

} // end namespace CNORXZInternal


#endif
