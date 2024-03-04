// -*- C++ -*-
/**
   
   @file opt/mpi/tests/l_unit_test.cc
   @brief L unit tests.

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
	    mData = MArray<Arr<Int,4>>(yrange({mRaTR,mRaSR,mRaSR,mRaSR,mLocTR,mLocSR,mLocSR,mLocSR,mSpinR,mColorR,mSpinR,mColorR}));
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
	    mView = Slice<Arr<Int,4>>(yrange({mRaTR,mLocTR,mRaSR,mLocSR,mRaSR,mLocSR,mRaSR,mLocSR,mSpinR,mColorR,mSpinR,mColorR}), &mData, mViewFormat, 0);
	    mLocSI = CIndex(mLocSR);
	    mLocTI = CIndex(mLocTR);
	    mRaSI = CIndex(mRaSR);
	    mRaTI = CIndex(mRaTR);
	    mSpinI = SIndex<SizeT,4>(mSpinR);
	    mColorI = SIndex<SizeT,3>(mColorR);
	}
	
	Sptr<SRange<SizeT,4>> mSpinR;
	Sptr<SRange<SizeT,3>> mColorR;
	Sptr<CRange> mSpatialR;
	Sptr<CRange> mTemporalR;
	Sptr<CRange> mLocSR;
	Sptr<CRange> mLocTR;
	Sptr<CRange> mRaSR;
	Sptr<CRange> mRaTR;

	MArray<Arr<Int,4>> mData;
	Slice<Arr<Int,4>> mView;
	YFormat mDataFormat;
	YFormat mViewFormat;

	CIndex mLocSI;
	CIndex mLocTI;
	CIndex mRaSI;
	CIndex mRaTI;
	SIndex<SizeT,4> mSpinI;
	SIndex<SizeT,3> mColorI;
	MIndex<CIndex,CIndex> mSpatialI;
	MIndex<CIndex,CIndex> mTemporalI;
	//GMIndex<Arr<SizeT,2>,CIndex,CIndex> mSpatialDI;
	//GMIndex<Arr<SizeT,2>,CIndex,CIndex> mTemporalDI;
    };

    TEST_F(L_Test, Basic)
    {
	EXPECT_EQ(mData.size(), mView.size());
    }

    TEST_F(L_Test, Assign)
    {
	auto x0l = std::make_shared<CIndex>(mLocTI);
	auto x0r = std::make_shared<CIndex>(mRaTI);
	auto x0 = gmindexPtr(x0r*x0l);
	auto x0a = gmindexPtr(x0r*x0l);
	auto x1l = std::make_shared<CIndex>(mLocSI);
	auto x1r = std::make_shared<CIndex>(mRaSI);
	auto x1 = gmindexPtr(x1r*x1l);
	auto x1a = gmindexPtr(x1r*x1l);
	auto x2l = std::make_shared<CIndex>(mLocSI);
	auto x2r = std::make_shared<CIndex>(mRaSI);
	auto x2 = gmindexPtr(x2r*x2l);
	auto x2a = gmindexPtr(x2r*x2l);
	auto x3l = std::make_shared<CIndex>(mLocSI);
	auto x3r = std::make_shared<CIndex>(mRaSI);
	auto x3 = gmindexPtr(x3r*x3l);
	auto x3a = gmindexPtr(x3r*x3l);
	auto x = gmindexPtr(x0*x1*x2*x3);
	auto xx = gmindexPtr(x0a*x1a*x2a*x3a);
	auto al = std::make_shared<SIndex<SizeT,4>>(mSpinI);
	auto be = std::make_shared<SIndex<SizeT,4>>(mSpinI);
	auto a = std::make_shared<SIndex<SizeT,3>>(mColorI);
	auto b = std::make_shared<SIndex<SizeT,3>>(mColorI);

	EXPECT_TRUE(x0->formatIsTrivial());
	EXPECT_TRUE(x1->formatIsTrivial());
	EXPECT_TRUE(x2->formatIsTrivial());
	EXPECT_TRUE(x3->formatIsTrivial());
	EXPECT_TRUE(x->formatIsTrivial());

	auto fs = [&](SizeT i) { return i >= mSpatialR->size()/2 ? static_cast<Int>(i - mSpatialR->size()) : static_cast<Int>(i); };
	auto ft = [&](SizeT i) { return i >= mTemporalR->size()/2 ? static_cast<Int>(i - mTemporalR->size()) : static_cast<Int>(i); };
        EXPECT_TRUE(mindexPtr(xx*al*a*be*b)->formatIsTrivial());

	mView(gmindexPtr(xx*al*a*be*b)) = operation( [](Int a0, Int a1, Int a2, Int a3) { Arr<Int,4> x{a0,a1,a2,a3}; return x; },
						     mapop(x0, ft), mapop(x1, fs), mapop(x2, fs), mapop(x3, fs) );
	EXPECT_FALSE(xx->formatIsTrivial());
	for(*x = 0; x->lex() != x->lmax().val(); ++(*x)){
	    Arr<Int,4> m{ ft(x0->lex()), fs(x1->lex()), fs(x2->lex()), fs(x3->lex()) };
	    EXPECT_EQ( mView[x*al*a*be*b], m );
	}
    }
}
