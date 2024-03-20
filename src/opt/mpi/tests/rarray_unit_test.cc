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
	    Vector<Double> vec = Numbers::get(0,size/4+10);
	    Vector<Double> data(size);
	    Vector<Double> mData(size*4);
	    const SizeT myrank = getRankNumber();
	    for(SizeT i = 0; i != size; ++i){
		data[i] = vec[i] * vec[i+myrank] / vec[i+2*myrank];
		mData[i + size*myrank] = data[i];
		MPI_Bcast(mData.data() + size*i, size, MPI_DOUBLE, i, MPI_COMM_WORLD);
	    }
	    mLoc = std::make_shared<MArray<Double>>( mRRange->sub(1), data);
	    mA = RCArray<Double>(mLoc, mGeom);
	}

	RangePtr mXRange;
	RangePtr mTRange;
	RangePtr mGRange;
	RangePtr mGeom;
	RangePtr mRRange;
	Sptr<MArray<Double>> mLoc;
	RCArray mA;
	Vector<Double> mData;
    };

    TEST_F(RArray_Test, Basics)
    {
	EXPECT_EQ(mA.size(), mRRange->size());
    }
    
    TEST_F(RArray_Test, GlobalIterate)
    {
	const SizeT size = mRRange->sub(1)->size();
	auto e = mA.end();
	for(auto i = mA.begin(); i != e; ++i){
	    EXPECT_EQ(*i, mData[i.rank()*size + i.local().pos()]);
	}
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment( new MPIEnv(argc, argv) );
    return RUN_ALL_TESTS();
}
