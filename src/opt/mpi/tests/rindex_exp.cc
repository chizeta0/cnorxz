// -*- C++ -*-

// experiment...

#include <cstdlib>
#include <iostream>

#include "cnorxz.h"
#include "cnorxz_mpi.h"
#include "test_numbers.h"
#include "rrange.cc.h"

namespace
{
    using namespace CNORXZ;
    using Test::Numbers;
    using namespace CNORXZ::mpi;

    class Env
    {
    public:
	
	Env()
	{
	    CXZ_ASSERT(getNumRanks() == 4, "exptected 4 ranks");
	    Vector<Int> xs(12);
	    Vector<Int> ts(16);
	    for(SizeT i = 0; i != xs.size(); ++i){
		const Int x = static_cast<Int>(i) - static_cast<Int>(xs.size()/2);
		xs[i] = x;
	    }
	    for(SizeT i = 0; i != ts.size(); ++i){
		const Int t = static_cast<Int>(i) - static_cast<Int>(ts.size()/2);
		ts[i] = t;
	    }
	    mSRange = CRangeFactory(4).create();
	    mXRange = URangeFactory<Int>(xs).create();
	    mTRange = URangeFactory<Int>(ts).create();
	    Vector<RangePtr> rs { mTRange, mXRange, mXRange, mXRange };
	    mGRange = YRangeFactory(rs).create();
	    RangePtr g1 = CRangeFactory(1).create();
	    RangePtr g2 = CRangeFactory(2).create();
	    Vector<RangePtr> gs { g2, g1, g1, g2 };
	    mGeom = YRangeFactory(gs).create();
	    mRRange = rrange(mGRange, mGeom);
	}

	RangePtr mSRange;
	RangePtr mXRange;
	RangePtr mTRange;
	RangePtr mGRange;
	RangePtr mGeom;
	RangePtr mRRange;
    };

    template <class Index, class IndexR>
    class IndexMap : public Index
    {
    public:
	typedef typename Index::IB IB;
	typedef typename Index::RangeType RangeType;

	DEFAULT_MEMBERS(IndexMap);

	IndexMap(const Sptr<Index>& i, const Vector<SizeT>& map, const Sptr<IndexR>& r) :
	    Index(*i), mI(i), mR(r), mMap(map) {}

	constexpr decltype(auto) id() const
	{
	    return mI->id();
	}

	template <SizeT I>
	decltype(auto) stepSize(const IndexId<I>& id) const
	{
	    // TODO: new Pos Type!!!
	    //assert(0);
	    return FPos( mI->stepSize(id).val(), mMap.data(), mI->lmax().val(), mR->lmax().val() );
	}

    private:
	Sptr<Index> mI;
	Sptr<IndexR> mR;
	Vector<SizeT> mMap;
    };

    template <class Index, class IndexR>
    constexpr decltype(auto) shift(const Sptr<Index>& i, const Sptr<IndexR>& r)
    {
	// only one-dim indices!!!
	auto j = *i;
	Vector<SizeT> mp(j.lmax().val());
	const SizeT L = i->lmax().val()*r->lmax().val();
	for(j = 0; j.lex() != j.lmax().val(); ++j){
	    mp[j.lex()] = ( j.lex() + 1 + L) % L;
	}
	return std::make_shared<IndexMap<Index,IndexR>>(i, mp, r);
    }
    
    template <class Index>
    class PosOp : public COpInterface<PosOp<Index>>
    {
    public:
	typedef COpInterface<PosOp<Index>> OI;

	constexpr PosOp() = default;

	constexpr PosOp(const Sptr<Index>& i, SizeT block) :
	    mMyrank(getRankNumber()), mI(i), mBlock(block) {}

	template <class PosT>
	constexpr decltype(auto) operator()(const PosT& pos) const
	{
	    return static_cast<SizeT>(pos)+mMyrank*mBlock;
	}

	constexpr decltype(auto) operator()() const
	{
	    return static_cast<SizeT>(mMyrank*mBlock);
	}

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const
	{
	    return mI->stepSize(id);
	}

    private:
	SizeT mMyrank;
	Sptr<Index> mI;
	SizeT mBlock;
    };

    template <class Index>
    constexpr decltype(auto) posop(const Sptr<Index>& i, SizeT block)
    {
	return PosOp<Index>(i, block);
    }
}

void run2(const Env& env)
{
    const SizeT myrank = getRankNumber();
    const SizeT Nranks = getNumRanks();
    
    typedef UIndex<Int> UI;
    typedef MIndex<UI,UI,UI,UI> LocI;
    typedef MIndex<CIndex,CIndex,CIndex,CIndex> RankI;
    auto rgi = std::make_shared<RIndex<LocI,RankI>>(env.mRRange);
    auto rgj = std::make_shared<RIndex<LocI,RankI>>(env.mRRange);
    auto rgk = std::make_shared<RIndex<LocI,RankI>>(env.mRRange);
    LocI gi(env.mGRange);
    LocI gj(env.mGRange);
    auto ri = std::make_shared<RankI>(env.mGeom);
    constexpr auto C0 = CSizeT<0> {};
    constexpr auto C1 = CSizeT<1> {};
    constexpr auto C2 = CSizeT<2> {};
    constexpr auto C3 = CSizeT<3> {};

    const SizeT LSize = env.mRRange->sub(1)->size();
    
    const SizeT blocks = env.mSRange->size();
    Vector<Double> data(LSize*blocks);
    Vector<Double> buf;
    Vector<Double*> map(env.mRRange->size(),nullptr);
    for(SizeT i = 0; i != data.size(); ++i){
	data[i] = static_cast<Double>(LSize*myrank*blocks+i);
    }
    Vector<Vector<SizeT>> cnt(Nranks);
    for(auto& c: cnt){
	c.resize(Nranks);
    }
    Vector<Vector<Double>> sendbuf(Nranks);
    for(auto& sb: sendbuf){
	sb.reserve(data.size());
    }

    auto srgi = rindexPtr( mindexPtr(
	shift(rgi->local()->pack()[C0], ri->pack()[C0]),
	rgi->local()->pack()[C1],
	shift(rgi->local()->pack()[C2], ri->pack()[C2]),
	shift(rgi->local()->pack()[C3], ri->pack()[C3])
    ), ri );
    VCHECK(srgi->lmax().val());
    
    *rgj = 0;
    while(rgj->rank() != 1){
	++*rgj;
    }
    *rgj->local() = 0;
    rgi->ifor( operation
	       ( [&](SizeT p) {
		   gj = rgj->lex();
		   *gj.pack()[C0] = (gj.pack()[C0]->lex() + 1) % gj.pack()[C0]->lmax().val();
		   *gj.pack()[C2] = (gj.pack()[C2]->lex() + 1) % gj.pack()[C2]->lmax().val();
		   *gj.pack()[C3] = (gj.pack()[C3]->lex() + 1) % gj.pack()[C3]->lmax().val();
		   gj();
		   *rgk = gj.lex();
		   if(myrank == 1){
		       std::cout << p << " " << rgk->pos() << " " << rgj->pos() << std::endl;
		   }
		   ++*rgj->local();
		   (*rgj)();
	       } , posop(srgi, rgi->local()->pmax().val()) ) ,
	       NoF {} )();
    
    MPI_Barrier(MPI_COMM_WORLD);
}

void run1(const Env& env)
{
    const SizeT myrank = getRankNumber();
    const SizeT Nranks = getNumRanks();
    
    typedef UIndex<Int> UI;
    typedef MIndex<UI,UI,UI,UI> LocI;
    typedef MIndex<CIndex,CIndex,CIndex,CIndex> RankI;
    RIndex<LocI,RankI> rgi(env.mRRange);
    RIndex<LocI,RankI> rgj(env.mRRange);
    LocI gi(env.mGRange);
    LocI gj(env.mGRange);
    RankI ri(env.mGeom);
    constexpr auto C0 = CSizeT<0> {};
    constexpr auto C2 = CSizeT<2> {};
    constexpr auto C3 = CSizeT<3> {};

    const SizeT LSize = env.mRRange->sub(1)->size();
    
    const SizeT blocks = env.mSRange->size();
    Vector<Double> data(LSize*blocks);
    Vector<Double> buf;
    Vector<Double*> map(env.mRRange->size(),nullptr);
    for(SizeT i = 0; i != data.size(); ++i){
	data[i] = static_cast<Double>(LSize*myrank*blocks+i);
    }
    Vector<Vector<SizeT>> cnt(Nranks);
    for(auto& c: cnt){
	c.resize(Nranks);
    }
    Vector<Vector<Double>> sendbuf(Nranks);
    for(auto& sb: sendbuf){
	sb.reserve(data.size());
    }

    // First loop: setup send buffer
    for(rgi = 0, gi = 0; rgi.lex() != rgi.lmax().val(); ++rgi, ++gi){
	gj = gi.lex();
	*gj.pack()[C0] = (gj.pack()[C0]->lex() + 1) % gj.pack()[C0]->lmax().val();
	*gj.pack()[C2] = (gj.pack()[C2]->lex() + 1) % gj.pack()[C2]->lmax().val();
	*gj.pack()[C3] = (gj.pack()[C3]->lex() + 1) % gj.pack()[C3]->lmax().val();
	gj();
	rgj = gj.lex();
	if(rgi.rank() != rgj.rank()){
	    if(rgj.rank() == myrank){
		// j = sender
		const Double* d = data.data()+rgj.local()->pos()*blocks;
		assert(static_cast<Int>(*d) % 4 == 0);
		sendbuf[rgi.rank()].insert(sendbuf[rgi.rank()].end(), d, d+blocks);
	    }
	    ++cnt[rgi.rank()][rgj.rank()];
	}
    }
    
    // Initialize target buffer:
    SizeT bufsize = 0;
    for(const auto& c: cnt[myrank]){
	bufsize += c*blocks;
    }
    buf.resize(bufsize);
    
    // Transfer data:
    for(SizeT o = 0; o != Nranks; ++o){
	const SizeT dstr = (myrank + o) % Nranks;
	const SizeT srcr = (myrank - o + Nranks) % Nranks;
	SizeT off = 0;
	for(SizeT p = 0; p != srcr; ++p){
	    off += cnt[myrank][p];
	}
	MPI_Status stat;
	MPI_Sendrecv(sendbuf[dstr].data(), cnt[dstr][myrank]*blocks, MPI_DOUBLE, dstr, 0,
		     buf.data()+off*blocks, cnt[myrank][srcr]*blocks, MPI_DOUBLE, srcr, 0,
		     MPI_COMM_WORLD, &stat);
    }
    Vector<Vector<SizeT>> ext = cnt;

    // Second loop: Assign map to target buffer positions:
    for(auto& c: cnt){
	c = Vector<SizeT>(Nranks,0);
    }
    for(rgi = 0, gi = 0; rgi.lex() != rgi.lmax().val(); ++rgi, ++gi){
	gj = gi.lex();
	*gj.pack()[C0] = (gj.pack()[C0]->lex() + 1) % gj.pack()[C0]->lmax().val();
	*gj.pack()[C2] = (gj.pack()[C2]->lex() + 1) % gj.pack()[C2]->lmax().val();
	*gj.pack()[C3] = (gj.pack()[C3]->lex() + 1) % gj.pack()[C3]->lmax().val();
	gj();
	rgj = gj.lex();

	if(rgi.rank() == myrank){
	    if(rgj.rank() != myrank){
		SizeT off = 0;
		for(SizeT p = 0; p != rgj.rank(); ++p){
		    off += ext[myrank][p];
		}
		map[rgj.pos()] = buf.data()+off*blocks + cnt[rgi.rank()][rgj.rank()]*blocks;
		++cnt[rgi.rank()][rgj.rank()];
	    }
	    map[rgi.pos()] = data.data() + rgi.local()->pos()*blocks;
	}
    }

    // Third loop: Check:
    for(rgi = 0, gi = 0; rgi.lex() != rgi.lmax().val(); ++rgi, ++gi){
	gj = gi.lex();
	*gj.pack()[C0] = (gj.pack()[C0]->lex() + 1) % gj.pack()[C0]->lmax().val();
	*gj.pack()[C2] = (gj.pack()[C2]->lex() + 1) % gj.pack()[C2]->lmax().val();
	*gj.pack()[C3] = (gj.pack()[C3]->lex() + 1) % gj.pack()[C3]->lmax().val();
	gj();
	rgj = gj.lex();

	if(rgi.rank() == myrank){
	    assert(map.data()[rgj.pos()] != nullptr);

	    const Double vn = *map[rgj.pos()]/blocks;
	    const SizeT xp = static_cast<SizeT>(vn);
	    const SizeT orank = xp / env.mRRange->sub(1)->size();
	    assert(env.mRRange->sub(1)->size() == 16*12*12*12/4);
	    if(myrank == 0){
		std::cout << " pos = " << rgj.pos() << " , val = " << *map[rgj.pos()]
			  << " , val_norm = " << vn << " , origin rank =  "
			  << orank << std::endl;
	    }
	    assert(orank == rgj.rank());
	    assert(vn == rgj.pos());
	}
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    Env env;
    run1(env);
    run2(env);
    
    MPI_Finalize();
    return 0;
}
