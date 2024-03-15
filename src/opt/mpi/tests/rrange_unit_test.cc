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

    class RRange_Test : public ::testing::Test
    {
    protected:

	RRange_Test()
	{
	    CXZ_ASSERT(getNumRanks() == 4, "exptected 4 ranks");
	    Vector<Int> xs(12);
	    Vector<Int> ts(16);
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
    
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    const int ret = RUN_ALL_TESTS();
    MPI_Finalize();
    return ret;
}