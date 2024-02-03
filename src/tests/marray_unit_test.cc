// -*- C++ -*-
/**
   
   @file tests/marray_unit_test.cc
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

    class MA_1D_Test : public ::testing::Test
    {
    protected:

	MA_1D_Test()
	{
	    mSize = 7;
	    mCR1 = CRangeFactory(mSize).create();
	    mA = MArray<Double>(mCR1, Numbers::get(0,mSize));
	}

	SizeT mSize;
	RangePtr mCR1;
	MArray<Double> mA;
    };

    class MA_2D_Test : public ::testing::Test
    {
    protected:

	MA_2D_Test()
	{
	    mSize = 5;
	    mStrMeta = { "another", "test", "string", "vector", "for", "this", "Test" };
	    mCR1 = CRangeFactory(mSize).create();
	    mUR1 = URangeFactory<String>(mStrMeta).create();
	    const SizeT ssize = mStrMeta.size();
	    const SizeT size = mSize * ssize;
	    mA = MArray<Double>(mCR1*mUR1, Numbers::get(0,size));
	    mA2 = MArray<Double>(mUR1*mCR1, Numbers::get(0,size));
	}

	SizeT mSize;
	Vector<String> mStrMeta;
	RangePtr mCR1;
	RangePtr mUR1;
	MArray<Double> mA;
	MArray<Double> mA2;
    };

    TEST_F(MA_1D_Test, Basics)
    {
	auto crx = std::dynamic_pointer_cast<CRange>(mCR1);
	EXPECT_EQ(mA.size(), mSize);
	EXPECT_FALSE(mA.isView());
	
	auto ei = crx->end();
	EXPECT_EQ(ei.lex(), mSize);
	for(auto i = crx->begin(); i != ei; ++i){
	    EXPECT_EQ(i.lex(), i.pos());
	    EXPECT_EQ(mA[i], mA.data()[i.lex()]);
	    EXPECT_EQ(mA.at(i), mA.data()[i.lex()]);
	}
	EXPECT_THROW({mA.at(ei);}, std::runtime_error);
    }

    TEST_F(MA_1D_Test, Slice)
    {
	auto crx = std::dynamic_pointer_cast<CRange>(mCR1);
	auto beg = crx->begin() + 2;
	auto last = crx->begin() + 5;

	//const auto& a = mA;
	auto sl = mA.sl(beg,last);
	EXPECT_EQ(sl->size(), last.lex()-beg.lex()+1);

	SizeT c = 0;
	auto ii = crx->begin();
	for(auto i = sl->begin() ; i != sl->end(); ++i, ++c, ++ii){
	    EXPECT_EQ(i.lex(), c);
	    EXPECT_EQ(ii.lex(), c);
	    EXPECT_EQ((*sl)[i], mA.data()[i.lex()+beg.lex()]);
	    EXPECT_EQ((*sl)[ii], mA.data()[i.lex()+beg.lex()]);
	    EXPECT_EQ(sl->at(i), mA.data()[i.lex()+beg.lex()]);
	    EXPECT_EQ(sl->at(ii), mA.data()[i.lex()+beg.lex()]);
	}
    }

    TEST_F(MA_2D_Test, Basics)
    {
	const SizeT ssize = mStrMeta.size();
	const SizeT size = mSize * ssize;
	EXPECT_EQ(mA.range()->dim(), 2u);
	EXPECT_EQ(mA.size(), size);
	EXPECT_EQ(mA.range()->sub(0), mCR1);
	EXPECT_EQ(mA.range()->sub(1), mUR1);
	auto cr1x = std::dynamic_pointer_cast<CRange>(mCR1);
	auto ur1x = std::dynamic_pointer_cast<URange<String>>(mUR1);
	for(auto ci = cr1x->begin(); ci != cr1x->end(); ++ci){
	    for(auto ui = ur1x->begin(); ui != ur1x->end(); ++ui){
		const SizeT p = ci.lex()*ssize + ui.lex();
		EXPECT_EQ( mA[ci*ui], mA.data()[p] );
	    }
	}
    }

    TEST_F(MA_2D_Test, Operation)
    {
	auto uip1 = std::make_shared<UIndex<String>>(mUR1);
	auto cip1 = std::make_shared<CIndex>(mCR1);
	mA2(uip1*cip1) = mA(cip1*uip1);
	for(auto ui = mUR1->begin(); ui != mUR1->end(); ++ui){
	    for(auto ci = mCR1->begin(); ci != mCR1->end(); ++ci){
		EXPECT_EQ(mA2[ui*ci], mA[ci*ui]);
	    }
	}
    }
    
    TEST_F(MA_2D_Test, Slice)
    {
	auto crx = std::dynamic_pointer_cast<CRange>(mCR1);
	auto beg = crx->begin() + 2;
	auto last = crx->begin() + 5;

	auto uip1 = std::make_shared<UIndex<String>>(mUR1);
	auto cip1 = std::make_shared<CIndex>(mCR1);
	auto beg1 = (*cip1)+1;
	auto last1 = (*cip1)+3;
	auto beg2 = (*uip1)+2;
	auto last2 = (*uip1)+5;
	
	auto sl = mA.sl(mindex(beg1*beg2),mindex(last1*last2));
	EXPECT_EQ(sl->size(), (last1.lex()-beg1.lex()+1)*(last2.lex()-beg2.lex()+1));

	MIndex<PIndex<CIndex>,PIndex<UIndex<String>>> mi(sl->range());
	EXPECT_EQ(mi.lmax().val(), sl->size());
	auto i1 = *mi.pack()[CSizeT<0>{}];
	auto i2 = *mi.pack()[CSizeT<1>{}];
	EXPECT_EQ(i1.lmax().val(), last1.lex()-beg1.lex()+1);
	EXPECT_EQ(i2.lmax().val(), last2.lex()-beg2.lex()+1);
	for(i1 = 0; i1.lex() != i1.lmax().val(); ++ i1){
	    for(i2 = 0; i2.lex() != i2.lmax().val(); ++i2){
		const SizeT p = (i1.lex()+beg1.lex())*uip1->lmax().val() + i2.lex()+beg2.lex();
		EXPECT_EQ((*sl)[i1*i2], mA.data()[p]);
	    }
	}
    }

} // end namespace 

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
