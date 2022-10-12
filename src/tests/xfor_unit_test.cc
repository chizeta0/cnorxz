
#include "gtest/gtest.h"

#include "ranges/xfor/xfor.h"

namespace
{
    using namespace CNORXZ;

    template <class PosT1, class PosT2>
    constexpr auto mkMPos(const PosT1& a, const PosT2& b)
    {
	return MPos<PosT1,PosT2>(a,b);
    }

    class Pos_Test : public ::testing::Test
    {
    protected:

	static constexpr SizeT s1 = 7;
	static constexpr SizeT s2 = 3;
	static constexpr SizeT ss1 = 4;
	static constexpr SizeT ss2 = 2;
	
	Pos_Test()
	{
	    mUp1 = UPos(s1);
	    mUp2 = UPos(s2);
	}

	UPos mUp1;
	UPos mUp2;
	SPos<ss1> mS4p;
	SPos<ss2> mS2p;
    };

    TEST_F(Pos_Test, Basics)
    {
	EXPECT_EQ( mUp1.size(), 1u );
	EXPECT_EQ( mUp2.size(), 1u );
	EXPECT_EQ( mS4p.size(), 1u );
	EXPECT_EQ( mS2p.size(), 1u );

	EXPECT_EQ( mUp1.val(), s1 );
	EXPECT_EQ( mUp2.val(), s2 );
	EXPECT_EQ( mS4p.val(), ss1 );
	EXPECT_EQ( mS2p.val(), ss2 );
	    
	auto s6p = mS4p + mS2p;
	auto s8p = mS4p * mS2p;
	auto up3 = mS4p * mUp1;
	auto up4 = mUp1 * mS2p;
	auto up5 = mS4p + mUp1;
	auto up6 = mUp1 + mS2p;

	EXPECT_EQ( s6p.size(), 1u );
	EXPECT_EQ( s8p.size(), 1u );
	EXPECT_EQ( up3.size(), 1u );
	EXPECT_EQ( up4.size(), 1u );
	EXPECT_EQ( up5.size(), 1u );
	EXPECT_EQ( up6.size(), 1u );

	EXPECT_EQ( s6p.val(), ss1+ss2 );
	EXPECT_EQ( s8p.val(), ss1*ss2 );
	EXPECT_EQ( up3.val(), ss1*s1 );
	EXPECT_EQ( up4.val(), s1*ss2 );
	EXPECT_EQ( up5.val(), ss1+s1 );
	EXPECT_EQ( up6.val(), s1+ss2 );
    }

    TEST_F(Pos_Test, Multi)
    {
	auto mp1 = mkMPos(mS2p, mUp1);
	auto mp2 = mkMPos(mUp2, mS4p);
	auto mp3a = mp1 + mp2;
	auto mp3b = mp2 + mp1;
	auto mp4 = mp1 * mS2p;
	auto mp5 = mp2 * mUp1;

	EXPECT_EQ( mp1.size(), 2u );
	EXPECT_EQ( mp2.size(), 2u );
	EXPECT_EQ( mp3a.size(), 2u );
	EXPECT_EQ( mp3b.size(), 2u );
	EXPECT_EQ( mp4.size(), 2u );
	EXPECT_EQ( mp5.size(), 2u );

	EXPECT_EQ(mp1.val(), mS2p.val());
	EXPECT_EQ(mp1.next().val(), mUp1.val());
	EXPECT_EQ(mp2.val(), mUp2.val());
	EXPECT_EQ(mp2.next().val(), mS4p.val());
	EXPECT_EQ(mp3a.val(), mS2p.val() + mUp2.val());
	EXPECT_EQ(mp3a.next().val(), mUp1.val() + mS4p.val());
	EXPECT_EQ(mp3b.val(), mS2p.val() + mUp2.val());
	EXPECT_EQ(mp3b.next().val(), mUp1.val() + mS4p.val());
	EXPECT_EQ(mp4.val(), mS2p.val() * mS2p.val());
	EXPECT_EQ(mp4.next().val(), mUp1.val() * mS2p.val());
	EXPECT_EQ(mp5.val(), mUp2.val() * mUp1.val());
	EXPECT_EQ(mp5.next().val(), mS4p.val() * mUp1.val());
    }

    TEST_F(Pos_Test, Extend)
    {
	auto mp1 = mkMPos(mS2p, mUp1);
	auto mp2 = mkMPos(mUp2, mS4p);
	auto mp3 = mp1.extend(mUp2);
	auto mp4 = mp2.extend(mS2p);

	EXPECT_EQ( mp3.size(), 3u );
	EXPECT_EQ( mp4.size(), 3u );

	EXPECT_EQ( mp3.val(), mS2p.val() );
	EXPECT_EQ( mp3.next().val(), mUp1.val() );
	EXPECT_EQ( mp3.next().next().val(), mUp2.val() );

    	EXPECT_EQ( mp4.val(), mUp2.val() );
	EXPECT_EQ( mp4.next().val(), mS4p.val() );
	EXPECT_EQ( mp4.next().next().val(), mS2p.val() );
    }
    
    TEST_F(Pos_Test, Dyn)
    {
	DPos dp01(static_cast<UPos>(mS2p));
	DPos dp02(mUp1);
	
	DPos dp1(mkMPos(static_cast<UPos>(mS2p), mUp1));
	DPos dp2(mkMPos(mUp2, static_cast<UPos>(mS4p)));
	auto dp3a = dp1 + dp2;
	auto dp3b = dp2 + dp1;
	auto dp4 = dp1 * dp01;
	auto dp5 = dp2 * dp02;
	
	EXPECT_EQ(dp01.size(), 1u);
	EXPECT_EQ(dp02.size(), 1u);

	EXPECT_EQ(dp01.val(), mS2p.val());
	EXPECT_EQ(dp02.val(), mUp1.val());
	
	EXPECT_EQ(dp1.size(), 2u);
	EXPECT_EQ(dp2.size(), 2u);

	EXPECT_EQ(dp3a.size(), 2u);
	EXPECT_EQ(dp3b.size(), 2u);
	EXPECT_EQ(dp4.size(), 2u);
	EXPECT_EQ(dp5.size(), 2u);

	EXPECT_EQ(dp1.val(), mS2p.val());
	EXPECT_EQ(dp1.next().val(), mUp1.val());
	EXPECT_EQ(dp2.val(), mUp2.val());
	EXPECT_EQ(dp2.next().val(), mS4p.val());
	EXPECT_EQ(dp3a.val(), mS2p.val() + mUp2.val());
	EXPECT_EQ(dp3a.next().val(), mUp1.val() + mS4p.val());
	EXPECT_EQ(dp3b.val(), mS2p.val() + mUp2.val());
	EXPECT_EQ(dp3b.next().val(), mUp1.val() + mS4p.val());
	EXPECT_EQ(dp4.val(), mS2p.val() * mS2p.val());
	EXPECT_EQ(dp4.next().val(), mUp1.val() * mS2p.val());
	EXPECT_EQ(dp5.val(), mUp2.val() * mUp1.val());
	EXPECT_EQ(dp5.next().val(), mS4p.val() * mUp1.val());
	
    }
    
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
