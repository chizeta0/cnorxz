// -*- C++ -*-

#include <cstdlib>
#include <iostream>

#include "gtest/gtest.h"

#include "cnorxz.h"
#include "test_numbers.h"

namespace
{
    using namespace CNORXZ;

    class DA_1D_Test : public ::testing::Test
    {
    protected:

	DA_1D_Test()
	{
	    mSize = 7;
	    mCR1 = CRangeFactory(mSize).create();
	}

	SizeT mSize;
	RangePtr mCR1;
    };

    TEST_F(DA_1D_Test, Basics)
    {
	DArray<Double> a(mCR1, ::CNORXZ::Test::Numbers::get(0,mSize));
	auto crx = std::dynamic_pointer_cast<CRange>(mCR1);
	EXPECT_EQ(a.size(), mSize);
	
	auto ei = crx->end();
	EXPECT_EQ(ei.lex(), mSize);
	for(auto i = crx->begin(); i != ei; ++i){
	    EXPECT_EQ(i.lex(), i.pos());
	    EXPECT_EQ(a[i], a.data()[i.lex()]);
	    EXPECT_EQ(a.at(i), a.data()[i.lex()]);
	}
	EXPECT_THROW({a.at(ei);}, std::runtime_error);
    }

} // end namespace 

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
