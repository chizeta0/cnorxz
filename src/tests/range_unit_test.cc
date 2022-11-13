

#include <cstdlib>
#include <iostream>

#include "gtest/gtest.h"

//#include "cnorxz.h"
#include "ranges/ranges.h"

namespace
{

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

    class UR_Test : public ::testing::Test
    {
    protected:

	UR_Test()
	{
	    mMeta = { "These", "are", "test", "strings", "foo", "bar", "baz" };
	    std::sort(mMeta.begin(), mMeta.end(), std::less<String>());
	}

	Vector<String> mMeta;
    };
    
    TEST_F(CR_Test, Basics)
    {
	auto cr = CRangeFactory(mSize).create();
	auto crx = std::dynamic_pointer_cast<CRange>(cr);
	EXPECT_EQ(cr->size(), mSize);
	EXPECT_EQ(crx->size(), mSize);

	EXPECT_TRUE(crx->begin() != crx->end());
	EXPECT_FALSE(crx->begin() == crx->end());
	EXPECT_EQ(crx->begin().pos(), 0u);
	EXPECT_EQ(crx->end().pos(), mSize);

	EXPECT_TRUE(cr->begin() != cr->end());
	EXPECT_FALSE(cr->begin() == cr->end());
	EXPECT_EQ(cr->begin().pos(), 0u);
	EXPECT_EQ(cr->end().pos(), mSize);

	
	SizeT cnt = 0;
	auto endxi = cr->end();
	for(auto xi = cr->begin(); xi != endxi; ++xi){
	    EXPECT_EQ(xi.pos(), cnt);
	    EXPECT_TRUE(*xi == DType(cnt));
	    EXPECT_EQ((*xi).str(), std::to_string(cnt));
	    ++cnt;
	}
	
	SizeT cnt2 = 0;
	for(auto x: *crx){
	    EXPECT_EQ(x, cnt2++);
	}
	
    }

    TEST_F(UR_Test, Basics)
    {
	auto ur = URangeFactory<String>(mMeta).create();
	auto urx = std::dynamic_pointer_cast<URange<String>>(ur);
	EXPECT_EQ(ur->size(), mMeta.size());
	EXPECT_EQ(urx->size(), mMeta.size());
	
	EXPECT_TRUE(urx->begin() != urx->end());
	EXPECT_FALSE(urx->begin() == urx->end());
	EXPECT_EQ(urx->begin().pos(), 0u);
	EXPECT_EQ(urx->end().pos(), mMeta.size());

	EXPECT_TRUE(ur->begin() != ur->end());
	EXPECT_FALSE(ur->begin() == ur->end());
	EXPECT_EQ(ur->begin().pos(), 0u);
	EXPECT_EQ(ur->end().pos(), mMeta.size());

	SizeT cnt = 0;
	auto endxi = ur->end();
	for(auto xi = ur->begin(); xi != endxi; ++xi){
	    EXPECT_EQ(xi.pos(), cnt);
	    EXPECT_TRUE(*xi == DType(mMeta[cnt]));
	    EXPECT_EQ((*xi).str(), mMeta[cnt]);
	    ++cnt;
	}

	SizeT cnt2 = 0;
	for(auto x: *urx){
	    EXPECT_EQ(x, mMeta[cnt2++]);
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
