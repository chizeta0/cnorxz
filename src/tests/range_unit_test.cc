

#include <cstdlib>
#include <iostream>

#include "gtest/gtest.h"

//#include "cnorxz.h"
#include "ranges/ranges.h"

namespace {

    using namespace CNORXZ;
    
    class CR_Test : public ::testing::Test
    {
    protected:

	CR_Test()
	{
	    mSize = 7;
	}

	SizeT mSize;
    };

    TEST_F(CR_Test, Basics)
    {
	auto cr = CRangeFactory(mSize).create();
	auto crx = std::dynamic_pointer_cast<CRange>(cr);
	EXPECT_EQ(cr->size(), mSize);
	EXPECT_EQ(crx->size(), mSize);

	EXPECT_EQ(crx->begin() != crx->end(), true);
	EXPECT_EQ(crx->begin().pos(), 0u);
	EXPECT_EQ(crx->end().pos(), mSize);

	EXPECT_EQ(cr->begin() != cr->end(), true);
	EXPECT_EQ(cr->begin().pos(), 0u);
	EXPECT_EQ(cr->end().pos(), mSize);

	
	SizeT cnt = 0;
	auto endxi = cr->end();
	for(auto xi = cr->begin(); xi != endxi; ++xi){
	    EXPECT_EQ(xi.pos(), cnt);
	    EXPECT_EQ(*xi, cnt);
	    ++cnt;
	}
	
	SizeT cnt2 = 0;
	for(auto x: *crx){
	    EXPECT_EQ(x, cnt2++);
	}
	
    }

    // UR_Test
    // RCast_Test
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
