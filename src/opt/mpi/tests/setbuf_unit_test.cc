// -*- C++ -*-
/**
   
   @file opt/mpi/tests/setbuf_unit_test.cc
   @brief Setbuf unit tests.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#include <cstdlib>
#include <iostream>

#include "cnorxz.h"
#include "cnorxz_mpi.h"
#include "test_numbers.h"
#include "rrange.cc.h"
#include "mpi_env.h"

namespace
{
    using namespace CNORXZ;
    using Test::Numbers;
    using namespace CNORXZ::mpi;

    class Setbuf_Test : public ::testing::Test
    {
    protected:
	
	Setbuf_Test()
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



    TEST_F(Setbuf_Test, run)
    {
	const SizeT myrank = getRankNumber();
    
	typedef UIndex<Int> UI;
	typedef MIndex<UI,UI,UI,UI> LocI;
	typedef MIndex<CIndex,CIndex,CIndex,CIndex> RankI;
	auto rgi = std::make_shared<RIndex<LocI,RankI>>(mRRange);
	auto rgj = std::make_shared<RIndex<LocI,RankI>>(mRRange);
	auto rgk = std::make_shared<RIndex<LocI,RankI>>(mRRange);
	LocI gi(mGRange);
	LocI gj(mGRange);
	auto ri = std::make_shared<RankI>(mGeom);
	constexpr auto C0 = CSizeT<0> {};
	constexpr auto C2 = CSizeT<2> {};
	constexpr auto C3 = CSizeT<3> {};

	const SizeT LSize = mRRange->sub(1)->size();
	const SizeT blocks = mSRange->size();

	MArray<Double> data(mRRange->sub(1)*mSRange);
	for(SizeT i = 0; i != data.size(); ++i){
	    data.data()[i] = static_cast<Double>(LSize*myrank*blocks+i);
	}

	*rgj = 0;
	while(rgj->rank() != 1){
	    ++*rgj;
	}
	*rgj->local() = 0;

	Vector<Double> buf;
	Vector<const Double*> map(mRRange->size(),nullptr);

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
	setupBuffer(rgj, rgi, fmap, data, buf, map, mSRange->size());

	EXPECT_EQ(mRRange->sub(1)->size(), 16*12*12*12/4);
	const SizeT locsz = rgj->local()->lmax().val();
	const SizeT myrankoff = myrank*locsz;
	const SizeT mapsize = map.size();
	// Fourth loop: Check:
	for(*rgi = 0, gi = 0; rgi->lex() != rgi->lmax().val(); ++*rgi, ++gi){
	    gj = gi.lex();
	    *gj.pack()[C0] = (gj.pack()[C0]->lex() + 1) % gj.pack()[C0]->lmax().val();
	    *gj.pack()[C2] = (gj.pack()[C2]->lex() + 1) % gj.pack()[C2]->lmax().val();
	    *gj.pack()[C3] = (gj.pack()[C3]->lex() + 1) % gj.pack()[C3]->lmax().val();
	    gj();
	    *rgj = gj.lex();

	    if(rgi->rank() == myrank){
		const SizeT mpidx = (rgj->pos() - myrankoff + mapsize) % mapsize;
		VCHECK(mpidx);
		assert(mpidx < map.size());
		EXPECT_TRUE(map.data()[mpidx] != nullptr);
		if(map.data()[mpidx] == nullptr) continue;
		
		const Double vn = *map[mpidx]/blocks;
		const SizeT xp = static_cast<SizeT>(vn);
		const SizeT orank = xp / mRRange->sub(1)->size();
		if(myrank == 0){
		    std::cout << " pos = " << rgj->pos() << " , val = " << *map[mpidx]
			      << " , val_norm = " << vn << " , origin rank =  "
			      << orank << std::endl;
		}
		EXPECT_EQ(orank, rgj->rank());
		EXPECT_EQ(vn, rgj->pos());
	    }
	}
	MPI_Barrier(MPI_COMM_WORLD);
    }
    
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment( new MPIEnv(argc, argv) );
    return RUN_ALL_TESTS();
}
