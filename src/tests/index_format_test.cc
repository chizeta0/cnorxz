
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
	    mRange = yrange({cr11,mr105,cr13,sr2});
	}

	Vector<String> mMeta7;
	RangePtr mRange;
    };

    TEST_F(Format_Test, RangeCheck)
    {
	EXPECT_EQ(mRange->size(), 2*105*11*13);
	auto yi = mRange->begin();
	EXPECT_EQ(yi.lmax().val(), mRange->size());
	EXPECT_EQ(yi.pmax().val(), mRange->size());
    }

    TEST_F(Format_Test, CFormat)
    {
	CIndex ci(mRange);
	EXPECT_EQ(ci.lmax().val(), mRange->size());
	auto yi = mRange->begin();
	auto rfi = ci.reformat( yi.deepFormat(), yi.deepMax() );
	VCHECK(toString(yi.deepFormat()));
	VCHECK(toString(yi.deepMax()));
	EXPECT_EQ(rfi.lmax().val(), mRange->size());
    }
}
