
#include "gtest/gtest.h"

#include "ranges/xfor/xfor.h"

namespace
{
    using namespace CNORXZ;

    class Pos_Test : public ::testing::Test
    {
    protected:
	Pos_Test()
	{
	    mUp1 = UPos(7);
	    mUp2 = UPos(3);
	}

	UPos mUp1;
	UPos mUp2;
	SPos<4> mS4p;
	SPos<2> mS2p;
    };

    TEST_F(Pos_Test, Basics)
    {
	EXPECT_EQ( mUp1.size(), 1 );
	EXPECT_EQ( mUp2.size(), 1 );
	EXPECT_EQ( mS4p.size(), 1 );
	EXPECT_EQ( mS2p.size(), 1 );

	EXPECT_EQ( mUp1.val(), 7 );
	EXPECT_EQ( mUp2.val(), 3 );
	EXPECT_EQ( mS4p.val(), 4 );
	EXPECT_EQ( mS2p.val(), 2 );
	    
	auto s6p = mS4p + mS2p;
	auto s8p = mS4p * mS2p;
	auto up3 = mS4p * mUp1;
	auto up4 = mUp1 * mS2p;
	auto up5 = mS4p + mUp1;
	auto up6 = mUp1 + mS2p;

	EXPECT_EQ( s6p.size(), 1 );
	EXPECT_EQ( s8p.size(), 1 );
	EXPECT_EQ( up3.size(), 1 );
	EXPECT_EQ( up4.size(), 1 );
	EXPECT_EQ( up5.size(), 1 );
	EXPECT_EQ( up6.size(), 1 );

	EXPECT_EQ( s6p.val(), 6 );
	EXPECT_EQ( s8p.val(), 8 );
	EXPECT_EQ( up3.val(), 4*7 );
	EXPECT_EQ( up4.val(), 7*2 );
	EXPECT_EQ( up5.val(), 4+7 );
	EXPECT_EQ( up6.val(), 7+2 );
	
    }
}

