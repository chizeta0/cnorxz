
#include <cstdlib>
#include <iostream>

#include "gtest/gtest.h"

//#include "cnorxz.h"
#include "ranges/ranges.h"
#include "array/array.h"

namespace
{
    using namespace CNORXZ;

    class Format_Test : public ::testing::Test
    {
    protected:

	Format_Test()
	{
	    mMeta7 = { "These", "are", "test", "strings", "foo", "bar", "baz" };
	    auto sr2 = SRangeFactory<SizeT,2>(Arr<SizeT,2>{0,1}).create();
	    auto ur3x = std::dynamic_pointer_cast<URange<Real>>
		(URangeFactory<Real>(Vector<Real>{3.141,1.6,2.7}).create());
	    auto cr5x = std::dynamic_pointer_cast<CRange>(CRangeFactory(5).create());
	    auto ur7x = std::dynamic_pointer_cast<URange<String>>
		(URangeFactory<String>(mMeta7).create());
	    auto mr105 = mrange(ur3x,ur7x,cr5x);
	    auto cr11 = CRangeFactory(11).create();
	    auto cr13 = CRangeFactory(13).create();
	    auto cr17 = CRangeFactory(17).create();
	    mRange = yrange({cr11,mr105,cr13,sr2});
	    mRange2 = yrange({cr11,mr105,cr13,cr17,sr2});
	}

	Vector<String> mMeta7;
	RangePtr mRange;
	RangePtr mRange2;
    };

    TEST_F(Format_Test, RangeCheck)
    {
	EXPECT_EQ(mRange->size(), 2*105*11*13);
	EXPECT_EQ(mRange2->size(), 2*105*11*13*17);
	auto yi = mRange->begin();
	EXPECT_EQ(yi.lmax().val(), mRange->size());
	EXPECT_EQ(yi.pmax().val(), mRange->size());
	EXPECT_EQ(mRange->dim(), 4u);
    }

    TEST_F(Format_Test, CFormat)
    {
	CIndex ci(mRange);
	EXPECT_EQ(ci.lmax().val(), mRange->size());
	auto yi = mRange->begin();
	auto yi2 = mRange2->begin();
	ci.reformat( yi.deepFormat(), yi.deepMax() );
	VCHECK(toString(yi.deepFormat()));
	VCHECK(toString(yi.deepMax()));
	VCHECK(toString(yi2.deepFormat()));
	VCHECK(toString(yi2.deepMax()));
	EXPECT_EQ(ci.lmax().val(), mRange->size());
    }

    TEST_F(Format_Test, XFormat)
    {
	GMIndex<MFormat<4>,CIndex,CIndex,CIndex,CIndex> mi(mRange);
	MIndex<CIndex,CIndex,CIndex,CIndex,CIndex> mi2(mRange2);
	EXPECT_EQ(mi.lmax().val(), mRange->size());
	EXPECT_EQ(mi2.lmax().val(), mRange2->size());
	auto yi = indexAs<YIndex>( mRange->begin() );
	mi.reformat( yi.deepFormat(), yi.deepMax() );
	EXPECT_EQ(mi.lmax().val(), mRange->size());
	VCHECK(toString(mi.deepFormat()));
	VCHECK(toString(mi.deepMax()));
	VCHECK(toString(mi2.deepFormat()));
	VCHECK(toString(mi2.deepMax()));

	
	yi.setFormat( YFormat( Vector<SizeT>{ mi2.format()[CSizeT<0>{}].val(), mi2.format()[CSizeT<1>{}].val(), mi2.format()[CSizeT<2>{}].val(), mi2.format()[CSizeT<4>{}].val() } ) );
	VCHECK(toString(yi.deepFormat()));

	mi.reformat( yi.deepFormat(), yi.deepMax() );
	VCHECK(toString(mi.deepFormat()));
	VCHECK(toString(mi.deepMax()));
    }
}
