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

    class TwoDimTest : public ::testing::Test
    {
    protected:

	typedef MAT::SingleRange<char,MAT::RangeType::ANY> Range1dAny;
	typedef MAT::MultiRange<Range1dAny,Range1dAny> Range2dAny;
	typedef MAT::MultiArray<int,Range2dAny> MultiArray2dAny;
	
	TwoDimTest() : r1({'a','b','c'}),
		       r2({'a','b','c','d'}),
		       r(r1,r2),
		       ma(r, {-5,6,2,1,9,54,27,-7,-13,32,90,-67}) {}

	//virtual void SetUp();

	Range1dAny r1;
	Range1dAny r2;
	Range2dAny r;
	MultiArray2dAny ma;
	
    };
    
    TEST_F(OneDimTest, CorrectExtensions)
    {
	EXPECT_EQ(ma.size(), 5);
    }
    
    TEST_F(OneDimTest, CorrectAssigned)
    {
	auto i = ma.begin();
	EXPECT_EQ(ma[i = 0], -5);
	EXPECT_EQ(ma[i = 1], 6);
	EXPECT_EQ(ma[i = 2], 2);
	EXPECT_EQ(ma[i = 3], 1);
	EXPECT_EQ(ma[i = 4], 9);
    }

    TEST_F(TwoDimTest, CorrectExtensions)
    {
	EXPECT_EQ(ma.size(), 12);
    }

    
    TEST_F(TwoDimTest, CorrectAssigned)
    {    
	auto i = ma.begin();
	auto i1 = i.template getIndex<0>();
	auto i2 = i.template getIndex<1>();
	
	EXPECT_EQ(ma[i(i1 = 0, i2 = 0)], -5);
	EXPECT_EQ(ma[i(i1 = 0, i2 = 1)], 6);
	EXPECT_EQ(ma[i(i1 = 0, i2 = 2)], 2);
	EXPECT_EQ(ma[i(i1 = 0, i2 = 3)], 1);
	EXPECT_EQ(ma[i(i1 = 1, i2 = 0)], 9);
	EXPECT_EQ(ma[i(i1 = 1, i2 = 1)], 54);
	EXPECT_EQ(ma[i(i1 = 1, i2 = 2)], 27);
	EXPECT_EQ(ma[i(i1 = 1, i2 = 3)], -7);
	EXPECT_EQ(ma[i(i1 = 2, i2 = 0)], -13);
	EXPECT_EQ(ma[i(i1 = 2, i2 = 1)], 32);
	EXPECT_EQ(ma[i(i1 = 2, i2 = 2)], 90);
	EXPECT_EQ(ma[i(i1 = 2, i2 = 3)], -67);
    }
   
} // end namespace 

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
