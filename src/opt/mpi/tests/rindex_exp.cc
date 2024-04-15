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
	    Vector<Int> xs(L);
	    Vector<Int> ts(T);
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

	SizeT T = 16;
	SizeT L = 12;
	RangePtr mSRange;
	RangePtr mXRange;
	RangePtr mTRange;
	RangePtr mGRange;
	RangePtr mGeom;
	RangePtr mRRange;
    };


    template <class TarIndex, class SrcIndex, typename T>
    void setupBuffer(const Sptr<TarIndex>& rgj, const Sptr<SrcIndex>& rgi,
		     const Sptr<Vector<SizeT>>& fmap, const Vector<T>& data,
		     Vector<T>& buf, Vector<const T*>& map, const SizeT blocks)
    {
	const SizeT myrank = getRankNumber();
	const SizeT Nranks = getNumRanks();

	const SizeT mapsize = rgj->range()->size();
	map = Vector<const T*>(mapsize,nullptr);
	Vector<Vector<T>> sendbuf(Nranks);
	for(auto& sb: sendbuf){
	    sb.reserve(data.size());
	}
	Vector<Vector<SizeT>> request(Nranks);
	const SizeT locsz = rgi->local()->lmax().val();

	// First loop: setup send buffer
	rgi->ifor( mapXpr(rgj, rgi, fmap,
			  operation
			  ( [&](SizeT p, SizeT q) {
			      const SizeT r = p / locsz;
			      if(myrank != r){
				  request[r].push_back(p % locsz);
			      }
			  } , posop(rgj), posop(rgi) ) ) ,
		   NoF {} )();

	// transfer:
	Vector<SizeT> reqsizes(Nranks);
	SizeT bufsize = 0;
	Vector<Vector<SizeT>> ext(Nranks);
	for(auto& e: ext){
	    e.resize(Nranks);
	}
	for(SizeT i = 0; i != Nranks; ++i){
	    reqsizes[i] = request[i].size();
	    bufsize += reqsizes[i]*blocks;
	    ext[myrank][i] = reqsizes[i];
	}
	buf.resize(bufsize);
	MPI_Status stat;

	// transfer requests:
	for(SizeT o = 1; o != Nranks; ++o){
	    const SizeT dstr = (myrank + o) % Nranks;
	    const SizeT srcr = (myrank - o + Nranks) % Nranks;
	    SizeT sendsize = 0;
	    MPI_Sendrecv(reqsizes.data()+dstr, 1, MPI_UNSIGNED_LONG, dstr, 0,
			 &sendsize, 1, MPI_UNSIGNED_LONG, srcr, 0, MPI_COMM_WORLD, &stat);
	    ext[srcr][myrank] = sendsize;
	    Vector<SizeT> sendpos(sendsize);
	    MPI_Sendrecv(request[dstr].data(), reqsizes[dstr], MPI_UNSIGNED_LONG, dstr, 0,
			 sendpos.data(), sendsize, MPI_UNSIGNED_LONG, srcr, 0, MPI_COMM_WORLD, &stat);
	    sendbuf[srcr].resize(sendsize*blocks);
	    for(SizeT i = 0; i != sendsize; ++i){
		std::memcpy( sendbuf[srcr].data()+i*blocks, data.data()+sendpos[i]*blocks, blocks*sizeof(T) );
	    }
	}

	const MPI_Datatype dt = Typemap<T>::value();
	
	// transfer data:
	for(SizeT o = 1; o != Nranks; ++o){
	    const SizeT dstr = (myrank + o) % Nranks;
	    const SizeT srcr = (myrank - o + Nranks) % Nranks;
	    SizeT off = 0;
	    for(SizeT p = 0; p != srcr; ++p){
		off += ext[myrank][p];
	    }
	
	    MPI_Sendrecv(sendbuf[dstr].data(), ext[dstr][myrank]*blocks, dt, dstr, 0,
			 buf.data()+off*blocks, ext[myrank][srcr]*blocks, dt, srcr, 0,
			 MPI_COMM_WORLD, &stat);
	
	}

	// Second loop: Assign map to target buffer positions:
	Vector<SizeT> cnt(Nranks);
	rgi->ifor( mapXpr(rgj, rgi, fmap,
			  operation
			  ( [&](SizeT p, SizeT q) {
			      const SizeT r = p / locsz;
			      if(myrank != r){
				  SizeT off = 0;
				  for(SizeT s = 0; s != r; ++s){
				      off += ext[myrank][s];
				  }
				  map[p] = buf.data() + off*blocks + cnt[r]*blocks;
				  ++cnt[r];
			      }
			      map[q + myrank*locsz] = data.data() + q*blocks;
			  } , posop(rgj), posop(rgi) ) ), NoF {} )();
    }
}

void run2(const Env& env)
{
    const SizeT myrank = getRankNumber();
    //const SizeT Nranks = getNumRanks();
    
    typedef UIndex<Int> UI;
    typedef MIndex<UI,UI,UI,UI> LocI;
    typedef MIndex<CIndex,CIndex,CIndex,CIndex> RankI;
    const SizeT T = env.T;
    const SizeT L = env.L;
    auto rgi = std::make_shared<RIndex<LocI,RankI>>(env.mRRange);
    auto rgj = std::make_shared<RIndex<LocI,RankI>>(env.mRRange);
    auto rgk = std::make_shared<RIndex<LocI,RankI>>(env.mRRange);
    LocI gi(env.mGRange);
    LocI gj(env.mGRange);
    auto ri = std::make_shared<RankI>(env.mGeom);
    constexpr auto C0 = CSizeT<0> {};
    //constexpr auto C1 = CSizeT<1> {};
    constexpr auto C2 = CSizeT<2> {};
    constexpr auto C3 = CSizeT<3> {};

    const SizeT LSize = env.mRRange->sub(1)->size();
    const SizeT blocks = env.mSRange->size();

    Vector<Double> data(LSize*blocks);
    for(SizeT i = 0; i != data.size(); ++i){
	data[i] = static_cast<Double>(LSize*myrank*blocks+i);
    }

    *rgj = 0;
    while(rgj->rank() != 1){
	++*rgj;
    }
    *rgj->local() = 0;

    Vector<Double> buf;
    Vector<const Double*> map(env.mRRange->size(),nullptr);

    auto shift = [&](const auto& x){
	auto o = x;
	std::get<0>(o) += 1;
	if(std::get<0>(o) >= static_cast<int>(T)/2) { std::get<0>(o) -= T; }
	std::get<2>(o) += 1;
	if(std::get<2>(o) >= static_cast<int>(L)/2) { std::get<2>(o) -= L; }
	std::get<3>(o) += 1;
	if(std::get<3>(o) >= static_cast<int>(L)/2) { std::get<3>(o) -= L; }
	return o;
    };

    const Sptr<Vector<SizeT>> fmap = setupMap(rgj, rgi, shift);
    setupBuffer(rgj, rgi, fmap, data, buf, map, env.mSRange->size());

    // Third loop: Check:
    for(*rgi = 0, gi = 0; rgi->lex() != rgi->lmax().val(); ++*rgi, ++gi){
	gj = gi.lex();
	*gj.pack()[C0] = (gj.pack()[C0]->lex() + 1) % gj.pack()[C0]->lmax().val();
	*gj.pack()[C2] = (gj.pack()[C2]->lex() + 1) % gj.pack()[C2]->lmax().val();
	*gj.pack()[C3] = (gj.pack()[C3]->lex() + 1) % gj.pack()[C3]->lmax().val();
	gj();
	*rgj = gj.lex();

	if(rgi->rank() == myrank){
	    assert(map.data()[rgj->pos()] != nullptr);

	    const Double vn = *map[rgj->pos()]/blocks;
	    const SizeT xp = static_cast<SizeT>(vn);
	    const SizeT orank = xp / env.mRRange->sub(1)->size();
	    assert(env.mRRange->sub(1)->size() == 16*12*12*12/4);
	    if(myrank == 0){
		std::cout << " pos = " << rgj->pos() << " , val = " << *map[rgj->pos()]
			  << " , val_norm = " << vn << " , origin rank =  "
			  << orank << std::endl;
	    }
	    assert(orank == rgj->rank());
	    assert(vn == rgj->pos());
	}
    }
    CHECK;
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
