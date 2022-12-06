// -*- C++ -*-

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
	}

	SizeT mSize;
	RangePtr mCR1;
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
	}

	SizeT mSize;
	Vector<String> mStrMeta;
	RangePtr mCR1;
	RangePtr mUR1;
    };

    TEST_F(MA_1D_Test, Basics)
    {
	const MArray<Double> a(mCR1, Numbers::get(0,mSize));
	auto crx = std::dynamic_pointer_cast<CRange>(mCR1);
	EXPECT_EQ(a.size(), mSize);
	EXPECT_FALSE(a.isView());
	
	auto ei = crx->end();
	EXPECT_EQ(ei.lex(), mSize);
	for(auto i = crx->begin(); i != ei; ++i){
	    EXPECT_EQ(i.lex(), i.pos());
	    EXPECT_EQ(a[i], a.data()[i.lex()]);
	    EXPECT_EQ(a.at(i), a.data()[i.lex()]);
	}
	EXPECT_THROW({a.at(ei);}, std::runtime_error);
    }

    TEST_F(MA_2D_Test, Basics)
    {
	const SizeT ssize = mStrMeta.size();
	const SizeT size = mSize * ssize;
	const MArray<Double> a(mCR1*mUR1, Numbers::get(0,size));
	EXPECT_EQ(a.range()->dim(), 2u);
	EXPECT_EQ(a.size(), size);
	EXPECT_EQ(a.pmax(), size);
	EXPECT_EQ(a.range()->sub(0), mCR1);
	EXPECT_EQ(a.range()->sub(1), mUR1);
	auto cr1x = std::dynamic_pointer_cast<CRange>(mCR1);
	auto ur1x = std::dynamic_pointer_cast<URange<String>>(mUR1);
	for(auto ci = cr1x->begin(); ci != cr1x->end(); ++ci){
	    for(auto ui = ur1x->begin(); ui != ur1x->end(); ++ui){
		const SizeT p = ci.lex()*ssize + ui.lex();
		EXPECT_EQ( a[ci*ui], a.data()[p] );
	    }
	}
    }
    
} // end namespace 

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
