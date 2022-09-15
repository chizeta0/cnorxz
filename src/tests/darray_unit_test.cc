// -*- C++ -*-

#include <cstdlib>
#include <iostream>

#include "gtest/gtest.h"

#include "cnorxz.h"

namespace {

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
	DArray<double> a(mCR1);
	EXPECT_EQ(a.size(), mSize);
    }
    /*
    TEST_F(DA_1D_Test, ReFormat)
    {
	swapFactory<SRF>( rfbptr, { 'a', 'c', 'e', 'g', 'i' } );
	std::shared_ptr<SRange> sr2 = std::dynamic_pointer_cast<SRange>( rfbptr->create() );
	
	Array<double,DA_1D_Test::SRange> ma(srptr, vv);
	auto i = ma.cbegin();
	EXPECT_EQ( ma[ i.at('x') ], 3.141);

	auto ma2 = ma.format( sr2 );
	auto j = ma2.cbegin();
	
	EXPECT_EQ( ma[ j.at('a') ], 3.141);
	EXPECT_EQ( ma[ j.at('c') ], 2.718);
	EXPECT_EQ( ma[ j.at('e') ], 1.618);
	EXPECT_EQ( ma[ j.at('g') ], 0.693);
	EXPECT_EQ( ma[ j.at('i') ], 0.577);
    }
    */
} // end namespace 

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
