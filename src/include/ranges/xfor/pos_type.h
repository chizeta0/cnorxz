
#ifndef __cxz_pos_type_h__
#define __cxz_pos_type_h__

#include "base/base.h"

namespace CNORXZ
{

    template <class PosT>
    class CPosInterface
    {
    public:
	DEFAULT_MEMBERS(CPosInterface);

	PosT& THIS() { return static_cast<PosT&>(*this); }
	const PosT& THIS() const { return static_cast<const PosT&>(*this); }
	
	inline SizeT size() const { return THIS().size(); }
	inline auto val() const { return THIS().val(); }
	inline CPosInterface<PosT> operator+(const CPosInterface<PosT>& a) const
	{ return THIS() + a.THIS(); }
	inline CPosInterface<PosT> operator*(const CPosInterface<PosT>& a) const
	{ return THIS() * a.THIS(); }

	template <class P>
	inline auto extend(const CPosInterface<P>& a) const { return THIS().extend(a); }
    };

    template <class PosT>
    class PosInterface : public CPosInterface<PosT>
    {
    public:
	typedef CPosInterface<PosT> PI;
	
	DEFAULT_MEMBERS(PosInterface);

	template <class PosT1, class PosT2>
	inline PosInterface& set(const CPosInterface<PosT1>& a, const CPosInterface<PosT2>& b,
				 const CPosInterface<PosT3>& c)
	{ return PI::THIS().set(a,b,c); }

	template <class PosT1, class PosT2>
	inline PosInterface& operator()(const CPosInterface<PosT1>& a, const CPosInterface<PosT2>& b,
					const CPosInterface<PosT3>& c)
	{ return set(a,b,c); }
    };

    template <SizeT N>
    class SPos : public CPosInterface<SPos>
    {
    public:
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
    
    class UPos : public PosInterface<UPos>
    {
    private:
        SizeT mExt = 0;
    public:
	
	DEFAULT_MEMBERS(UPos);

	constexpr UPos(SizeT ext);
	
	inline SizeT size() const;
	inline const SizeT& val() const;

	template <class PosT>
	constexpr UPos operator+(const CPosInterface<PosT>& a) const;

        template <class PosT>
	constexpr UPos operator*(const CPosInterface<PosT>& a) const;
	
        template <class PosT>
        constexpr auto extend(const CPosInterface<PosT>& y) const;

	template <class PosT1, class PosT2, class PosT3>
	inline UPos& set(const CPosInterface<PosT1>& a, const CPosInterface<PosT2>& b,
			 const CPosInterface<PosT3>& c);
    };
    
    class FPos : public CPosInterface<FPos>
    {
    private:
	SizeT mExt = 0;
	const SizeT* mMap = nullptr;

    public:
	DEFAULT_MEMBERS(FPos);

	inline FPos(SizeT ext, const SizeT* map);

	constexpr SizeT size() const;
	inline const SizeT& val() const;
	
	template <class PosT1>
	inline UPos operator+(const CPosInterface<PosT1>& a) const;

	template <class PosT1>
	inline UPos operator*(const CPosInterface<PosT1>& a) const;

	template <class PosT1>
	inline auto extend(const CPosInterface<PosT1>& a) const;
    };

    template <SizeT N, SizeT... Ms>
    class SFPos : public CPosInterface<SFPos>
    {
    public:
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
		 public PosInterface<DPos>
    {
    private:
	DPosRef mNextRef;
    public:
	DEFAULT_MEMBERS(DPos);
	inline DPos(Uptr<VPosBase>&& a);
	
	template <class PosT>
	inline DPos(const PosT& a);

	inline SizeT size() const;
	inline const SizeT& val() const;
	inline const DPosRef& next() const;

	template <class PosT2>
	inline auto operator+(const MPos<PosT2>& a) const;

	inline auto operator+(const UPos& a) const;
	inline auto operator+(const DPos& a) const;
	inline auto operator+(const DPosRef& a) const;
	inline auto operator*(SizeT a) const;

	template <class PosT1>
	inline DPos extend(const PosT1& a) const;
	
	template <class PosT1, class PosT2>
	inline PosInterface& set(const CPosInterface<PosT1>& a,
				 const CPosInterface<PosT2>& b, sizeT c);
    };
    
    class DPosRef : public CPosInterface<DPosRef>
    {
    private:
	DPosRef mNextRef;
	const VPosBase* mC;
    public:
	DEFAULT_MEMBERS(DPosRef);
	inline DPosRef(const VPosBase* c);

	inline const T& operator*() const;
	inline const T* operator->() const;
	
	inline sizeT size() const;
	inline const SizeT& val() const;
	inline const DPosRef& next() const;

	template <class PosT2>
	inline auto operator+(const MPos<PosT2>& a) const;

	inline auto operator+(const UPos& a) const;
	inline auto operator+(const DPos& a) const;
	inline auto operator+(const DPosRef& a) const;
	inline auto operator*(SizeT a) const;

	template <class PosT1>
	inline DPos extend(const PosT1& a) const;
    };

    template <class PosT1, class PosT2>
    class MPos : public PosInterface<MPos<PosT1,PosT2>>
    {
    private:

	PosT1 mFirst;
	PosT2 mNext;
	
    public:
	
	//static constexpr SizeT NUM = PosT::SIZE;
        //static constexpr SizeT SIZE = NUM + 1;

	DEFAULT_MEMBERS(MPos);
	
	constexpr MPos(const CPosInterface<PosT1>& first,
		       const CPosInterface<PosT2>& next);

	inline const SizeT& val() const;
	inline const PosT& next() const;

	inline auto operator+(const MPos& in) const;
	inline auto operator*(SizeT in) const;

	template <class PosT3>
	inline auto extend(const PosInterface<PosT3>& y) const;

	template <class PosT3, class PosT4>
	inline PosInterface& set(const CPosInterface<PosT3>& a,
				 const CPosInterface<PosT4>& b, sizeT c);
    };    
    

} // end namespace CNORXZInternal


#endif
