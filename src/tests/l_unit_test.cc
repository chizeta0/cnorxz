// -*- C++ -*-
/**
   
   @file tests/l_unit_test.cc
   @brief MArray unit tests.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#include <cstdlib>
#include <iostream>

#include "gtest/gtest.h"

#include "cnorxz.h"
#include "test_numbers.h"

namespace
{
    using namespace CNORXZ;
    using Test::Numbers;

    class L_Test : public ::testing::Test
    {
    protected:
	
	L_Test()
	{
	    auto a4 = Arr<SizeT,4> { 0,1,2,3 }; 
	    auto a3 = Arr<SizeT,3> { 0,1,2 }; 
	    mSpinR = std::dynamic_pointer_cast<SRange<SizeT,4>> ( SRangeFactory<SizeT,4>(a4).create() );
	    mColorR = std::dynamic_pointer_cast<SRange<SizeT,3>> ( SRangeFactory<SizeT,3>(a3).create() );
	    mSpatialR = std::dynamic_pointer_cast<CRange> ( CRangeFactory(8).create() );
	    mTemporalR = std::dynamic_pointer_cast<CRange> ( CRangeFactory(24).create() );
	    mLocSR = std::dynamic_pointer_cast<CRange> ( CRangeFactory(4).create() );
	    mLocTR = std::dynamic_pointer_cast<CRange> ( CRangeFactory(8).create() );
	    mRaSR = std::dynamic_pointer_cast<CRange> ( CRangeFactory(2).create() );
	    mRaTR = std::dynamic_pointer_cast<CRange> ( CRangeFactory(3).create() );
	    mData = MArray<Double>(yrange({mRaTR,mRaSR,mRaSR,mRaSR,mLocTR,mLocSR,mLocSR,mLocSR,mSpinR,mColorR,mSpinR,mColorR}));
	    mDataFormat = mData.begin().format();
	    auto vf = mDataFormat.all();
	    vf[0] = mDataFormat[0];
	    vf[1] = mDataFormat[4];
	    vf[2] = mDataFormat[1];
	    vf[3] = mDataFormat[5];
	    vf[4] = mDataFormat[2];
	    vf[5] = mDataFormat[6];
	    vf[6] = mDataFormat[3];
	    vf[7] = mDataFormat[7];
	    mViewFormat = YFormat(vf);
	    mView = Slice<Double>(yrange({mRaTR,mLocTR,mRaSR,mLocSR,mRaSR,mLocSR,mRaSR,mLocSR,mSpinR,mColorR,mSpinR,mColorR}), &mData, mViewFormat, 0);
	}
	
	Sptr<SRange<SizeT,4>> mSpinR;
	Sptr<SRange<SizeT,3>> mColorR;
	Sptr<CRange> mSpatialR;
	Sptr<CRange> mTemporalR;
	Sptr<CRange> mLocSR;
	Sptr<CRange> mLocTR;
	Sptr<CRange> mRaSR;
	Sptr<CRange> mRaTR;

	MArray<Double> mData;
	Slice<Double> mView;
	YFormat mDataFormat;
	YFormat mViewFormat;
    };

    TEST_F(L_Test, Basic)
    {
	EXPECT_EQ(mData.size(), mView.size());
	VCHECK(mData.size());
    }
    
}
