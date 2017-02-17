// -*- C++ -*-

#include <cstdlib>
#include "gtest/gtest.h"
#include <iostream>

#include "multi_array_header.h"

namespace MAT = MultiArrayTools;

namespace {
    
    class OneDimTest : public ::testing::Test
    {
    protected:

	typedef MAT::SingleRange<char,MAT::RangeType::ANY> Range1dAny;
	typedef MAT::MultiArray<int,Range1dAny> MultiArray1dAny;
	
	OneDimTest() : r({'a','b','c','d','e'}), ma(r, {-5,6,2,1,9}) {}

	//virtual void SetUp();

	Range1dAny r;
	MultiArray1dAny ma;
	
    };

    TEST_F(OneDimTest, CorrectExtensions)
    {
	EXPECT_EQ(ma.size(), 5);
    }
    
    TEST_F(OneDimTest, CorrectAssigned)
    {
	EXPECT_EQ(ma[0], -5);
	EXPECT_EQ(ma[1], 6);
	EXPECT_EQ(ma[2], 2);
	EXPECT_EQ(ma[3], 1);
	EXPECT_EQ(ma[4], 9);
    }
   
} // end namespace 

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
