
#include "gtest/gtest.h"

#include "xpr/xpr.h"

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

    class For_Test : public ::testing::Test
    {
    protected:

	class TestXpr1
	{
	public:
	    constexpr TestXpr1(const IndexId<0>& id) : mId(id) {}
	    
	    template <class PosT>
	    inline SizeT operator()(const PosT& last) const
	    {
		const SizeT o = 1u;
		return o << last.val();
	    }

	    template <SizeT I>
	    inline UPos rootSteps(const IndexId<I>& id) const
	    {
		return UPos( mId == id ? 1u : 0u );
	    }

	private:
	    IndexId<0> mId;
	};

	class TestXpr2
	{
	public:
	    constexpr TestXpr2(const IndexId<0>& id, SizeT size) :
		mId(id), mSize(size), mCnt(size) {}
	    
	    template <class PosT>
	    inline void operator()(const PosT& last) const
	    {
		--mCnt;
		EXPECT_EQ(mCnt, mSize-last.val()-1);
	    }

	    template <SizeT I>
	    inline UPos rootSteps(const IndexId<I>& id) const
	    {
		return UPos( mId == id ? 1u : 0u );
	    }

	private:
	    IndexId<0> mId;
	    SizeT mSize;
	    mutable SizeT mCnt;
	};

	static constexpr SizeT sSize = 7u;

	For_Test()
	{
	    mSize = sSize;
	    mId1 = 10u;
	    mId2 = 11u;
	    mId3 = 12u;
	}

	SizeT mSize;
	PtrId mId1;
	PtrId mId2;
	PtrId mId3;
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
	EXPECT_EQ(dp1.sub().val(), mUp1.val());
	EXPECT_EQ(dp2.val(), mUp2.val());
	EXPECT_EQ(dp2.sub().val(), mS4p.val());
	EXPECT_EQ(dp3a.val(), mS2p.val() + mUp2.val());
	EXPECT_EQ(dp3a.sub().val(), mUp1.val() + mS4p.val());
	EXPECT_EQ(dp3b.val(), mS2p.val() + mUp2.val());
	EXPECT_EQ(dp3b.sub().val(), mUp1.val() + mS4p.val());
	EXPECT_EQ(dp4.val(), mS2p.val() * mS2p.val());
	EXPECT_EQ(dp4.sub().val(), mUp1.val() * mS2p.val());
	EXPECT_EQ(dp5.val(), mUp2.val() * mUp1.val());
	EXPECT_EQ(dp5.sub().val(), mS4p.val() * mUp1.val());
    }

    TEST_F(For_Test, For)
    {
	auto loop = mkFor(mSize, IndexId<0>(mId1), TestXpr1( IndexId<0>(mId1) ),
			  [](auto& o, const auto& e) { o += e; });

	const UPos rs = loop.rootSteps(IndexId<0>(mId1));
	EXPECT_EQ(rs.val(), 1u);
	const UPos rs2 = loop.rootSteps(IndexId<0>(mId2));
	EXPECT_EQ(rs2.val(), 0u);
	const SizeT res = loop();
	EXPECT_EQ(res, (1u << mSize) - 1u);

	auto loop2 = mkFor(mSize, IndexId<0>(mId1), TestXpr2( IndexId<0>(mId1), mSize ));
	loop2();
    }

    TEST_F(For_Test, SFor)
    {
	auto loop = mkSFor<sSize>(IndexId<0>(mId1), TestXpr1( IndexId<0>(mId1) ),
				  [](const auto& a, const auto& b) { return a + b; });

	const UPos rs = loop.rootSteps(IndexId<0>(mId1));
	EXPECT_EQ(rs.val(), 1u);
	const UPos rs2 = loop.rootSteps(IndexId<0>(mId2));
	EXPECT_EQ(rs2.val(), 0u);
	const SizeT res = loop();
	EXPECT_EQ(res, (1u << mSize) - 1u);

	auto loop2 = mkSFor<sSize>(IndexId<0>(mId1), TestXpr2( IndexId<0>(mId1), mSize ));
	loop2();
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
