// -*- C++ -*-
/**
   
   @file opt/mpi/tests/rarray_unit_test.cc
   @brief RArray unit tests.

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
#include "rarray.cc.h"
#include "mpi_env.h"

namespace
{
    using namespace CNORXZ;
    using Test::Numbers;
    using namespace CNORXZ::mpi;

    class RCArray_Test : public ::testing::Test
    {
    protected:

	RCArray_Test()
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
	    const SizeT size = ts.size()*xs.size()*xs.size()*xs.size();
	    const SizeT locsize = size/4;
	    Vector<Double> vec = Numbers::get(0,locsize+10);
	    Vector<Double> data(locsize);
	    mData.resize(size);
	    const SizeT myrank = getRankNumber();
	    for(SizeT i = 0; i != locsize; ++i){
		assert(i < data.size());
		data[i] = vec[i] * vec[i+myrank] / vec[i+2*myrank];
		assert(i + locsize*myrank < mData.size());
		mData[i + locsize*myrank] = data[i];
	    }
	    MPI_Barrier(MPI_COMM_WORLD);
	    for(SizeT r = 0; r != 4; ++r){
		MPI_Bcast(mData.data() + locsize*r, locsize, MPI_DOUBLE, r, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
	    }
	    mLoc = MArray<Double>( mRRange->sub(1), data);
	    mA = RCArray<Double>(mLoc, mGeom);
	}

	RangePtr mXRange;
	RangePtr mTRange;
	RangePtr mGRange;
	RangePtr mGeom;
	RangePtr mRRange;
        MArray<Double> mLoc;
	RCArray<Double> mA;
	Vector<Double> mData;
    };

    TEST_F(RCArray_Test, Basics)
    {
	EXPECT_EQ(mA.size(), mRRange->size());
    }

    TEST_F(RCArray_Test, GlobalIterate)
    {
	const SizeT size = mRRange->sub(1)->size();
	auto e = mA.end();
	for(auto i = mA.begin(); i != e; ++i){
	    const Double x = *i;
	    EXPECT_EQ(x, mData[i.rank()*size + i.local()->pos()]);
	}
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment( new MPIEnv(argc, argv) );
    return RUN_ALL_TESTS();
}
