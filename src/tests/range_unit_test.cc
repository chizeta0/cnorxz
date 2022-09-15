

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
	EXPECT_EQ(cr->size(), mSize);
    }

    // UR_Test
    // RCast_Test
}
