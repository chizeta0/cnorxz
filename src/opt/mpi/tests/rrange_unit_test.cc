// -*- C++ -*-
/**
   
   @file opt/mpi/tests/rrange_unit_test.cc
   @brief RRange unit tests.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#include <cstdlib>
#include <iostream>

#include "gtest/gtest.h"

#include "cnorxz.h"
#include "cnorxz_mpi.h"
#include "test_numbers.h"
#include "rrange.cc.h"

namespace
{
    using namespace CNORXZ;
    using Test::Numbers;
    using namespace CNORXZ::mpi;

    class MPIEnv : public ::testing::Environment
    {
    public:

	MPIEnv(int argc, char** argv) : mArgc(argc), mArgv(argv) {}
	
	virtual ~MPIEnv() override {}

	virtual void SetUp() override
	{
	    MPI_Init(&mArgc, &mArgv);
	}

	virtual void TearDown() override
	{
	    MPI_Finalize();
	}

    protected:
	int mArgc;
	char** mArgv;
    };
    
    class RRange_Test : public ::testing::Test
    {
    protected:

	RRange_Test()
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

	RangePtr mXRange;
	RangePtr mTRange;
	RangePtr mGRange;
	RangePtr mGeom;
	RangePtr mRRange;
    };

    TEST_F(RRange_Test, Basics)
    {
	EXPECT_EQ(mRRange->size(), mGRange->size());
    }

    TEST_F(RRange_Test, Local)
    {
	typedef UIndex<Int> UI;
	MIndex<UI,UI,UI,UI> mi(mRRange->sub(1));
	MIndex<CIndex,CIndex,CIndex,CIndex> ri(mRRange->sub(0));
	EXPECT_EQ(mi.lmax().val(), mGRange->size()/mRRange->sub(0)->size());
	EXPECT_EQ(ri.lmax().val(), getNumRanks());
	const SizeT rank = getRankNumber();
	ri = rank;
	iter<0,4>( [&](auto i) {
	    UI xi = *mi.pack()[i];
	    const SizeT max = xi.lmax().val();
	    const Int s = mGRange->sub(i)->size()/2;
	    const SizeT rx = ri.pack()[i]->lex();
	    for(; xi.lex() != max; ++xi){
		EXPECT_EQ(*xi, static_cast<Int>(xi.lex()) - s + static_cast<Int>(max*rx));
	    }
	}, NoF {} );
    }
    
    TEST_F(RRange_Test, Global)
    {
	RIndex<YIndex,YIndex> rgi(mRRange);
	YIndex gi(mGRange);
	for(SizeT c = 0; gi.lex() != gi.lmax().val(); ++gi, ++rgi, ++c) {
	    const String s1 = gi.stringMeta();
	    const String s2 = rgi.stringMeta();
	    EXPECT_EQ(rgi.lex(), c);
	    EXPECT_EQ(s1 ,s2);
	}
    }

    TEST_F(RRange_Test, Global2)
    {
	typedef UIndex<Int> UI;
	typedef MIndex<UI,UI,UI,UI> LocI;
	typedef MIndex<CIndex,CIndex,CIndex,CIndex> RankI;
	RIndex<LocI,RankI> rgi(mRRange);
	LocI gi(mGRange);
	for(SizeT c = 0; gi.lex() != gi.lmax().val(); ++gi, ++rgi, ++c) {
	    const String s1 = gi.stringMeta();
	    const String s2 = rgi.stringMeta();
	    EXPECT_EQ(rgi.lex(), c);
	    EXPECT_EQ(s1 ,s2);
	}
	rgi = 0;
	gi = 0;
	auto xi = gi;
	gi = 10;
	xi.at( gi.meta() );
	EXPECT_EQ( xi.lex(), gi.lex() );
	//VCHECK(toString(rgi.local()->meta()));
	VCHECK(rgi.local()->at( gi.meta() ).lex());
	/*
	auto j = rgi;
	for(auto i = rgi; i.lex() != i.lmax().val(); ++i){
	    j.at( i.meta() );
	    if(getRankNumber() == 0){
		EXPECT_EQ(i.lex(), j.lex());
		EXPECT_EQ(i.rank(), j.rank());
		EXPECT_EQ(i.local()->lex(), j.local()->lex());
	    }
	    if(i.lex() == 50) {
		break;
	    }
	}
	*/
    }
    
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment( new MPIEnv(argc, argv) );
    return RUN_ALL_TESTS();
}
