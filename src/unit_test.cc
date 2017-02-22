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

    class ReorderTest : public ::testing::Test
    {
    protected:
	
	typedef MAT::SingleRange<char,MAT::RangeType::ANY> Range1dAny;
	typedef MAT::MultiRange<Range1dAny,Range1dAny> Range2dAny;
	typedef MAT::MultiRange<Range1dAny,Range1dAny,Range1dAny> Range3dAny;
	typedef MAT::MultiArray<int,Range2dAny> MultiArray2dAny;
	typedef MAT::MultiArray<int,Range3dAny> MultiArray3dAny;

	ReorderTest() : r1({'a','b','c'}), r2({'a','b','c','d'}), r3({'a','b'}),
			ra(r1,r2), rb(r2,r1),
			r3da(r1,r2,r3), r3db(r1,r3,r2), r3dc(r3,r1,r2),
			ma(ra, {-5,6,2,1,9,54,27,-7,-13,32,90,-67}),
			ma3d(r3da, {-5,6,2,1,9,54,27,-7,-13,32,90,-67,
				    -10,16,-2,101,39,-64,81,-22,14,34,95,-62}) {}
	
	Range1dAny r1;
	Range1dAny r2;
	Range1dAny r3;
	Range2dAny ra;
	Range2dAny rb;
	Range3dAny r3da;
	Range3dAny r3db;
	Range3dAny r3dc;
	MultiArray2dAny ma;
	MultiArray3dAny ma3d;
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

    TEST_F(ReorderTest, ReorderingWorks2d)
    {
	MultiArray2dAny ma2(rb);
	auto i = ma2.begin();
	auto i1 = i.template getIndex<0>();
	auto i2 = i.template getIndex<1>();
	ma2("alpha","beta") = ma("beta","alpha");

	EXPECT_EQ(ma2[i(i1 = 0,i2 = 0)],-5);
	EXPECT_EQ(ma2[i(i1 = 1,i2 = 0)],6);
	EXPECT_EQ(ma2[i(i1 = 2,i2 = 0)],2);
	EXPECT_EQ(ma2[i(i1 = 3,i2 = 0)],1);	

	EXPECT_EQ(ma2[i(i1 = 0,i2 = 1)],9);
	EXPECT_EQ(ma2[i(i1 = 1,i2 = 1)],54);
	EXPECT_EQ(ma2[i(i1 = 2,i2 = 1)],27);
	EXPECT_EQ(ma2[i(i1 = 3,i2 = 1)],-7);

	EXPECT_EQ(ma2[i(i1 = 0,i2 = 2)],-13);
	EXPECT_EQ(ma2[i(i1 = 1,i2 = 2)],32);
	EXPECT_EQ(ma2[i(i1 = 2,i2 = 2)],90);
	EXPECT_EQ(ma2[i(i1 = 3,i2 = 2)],-67);
    }

    TEST_F(ReorderTest, ReorderingWorks3d_Test1)
    {
	MultiArray3dAny ma3d2(r3db);
	auto i = ma3d2.begin();
	auto i1 = i.template getIndex<0>();
	auto i2 = i.template getIndex<1>();
	auto i3 = i.template getIndex<2>();
	ma3d2("alpha","beta","gamma") = ma3d("alpha","gamma","beta");

	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 0,i3 = 0)],-5);
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 1,i3 = 0)],6);
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 0,i3 = 1)],2);
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 1,i3 = 1)],1);	
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 0,i3 = 2)],9);
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 1,i3 = 2)],54);
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 0,i3 = 3)],27);
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 1,i3 = 3)],-7);

	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 0,i3 = 0)],-13);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 1,i3 = 0)],32);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 0,i3 = 1)],90);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 1,i3 = 1)],-67);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 0,i3 = 2)],-10);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 1,i3 = 2)],16);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 0,i3 = 3)],-2);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 1,i3 = 3)],101);

	EXPECT_EQ(ma3d2[i(i1 = 2,i2 = 0,i3 = 0)],39);
	EXPECT_EQ(ma3d2[i(i1 = 2,i2 = 1,i3 = 0)],-64);
	EXPECT_EQ(ma3d2[i(i1 = 2,i2 = 0,i3 = 1)],81);
	EXPECT_EQ(ma3d2[i(i1 = 2,i2 = 1,i3 = 1)],-22);
	EXPECT_EQ(ma3d2[i(i1 = 2,i2 = 0,i3 = 2)],14);
	EXPECT_EQ(ma3d2[i(i1 = 2,i2 = 1,i3 = 2)],34);
	EXPECT_EQ(ma3d2[i(i1 = 2,i2 = 0,i3 = 3)],95);
	EXPECT_EQ(ma3d2[i(i1 = 2,i2 = 1,i3 = 3)],-62);
    }

    TEST_F(ReorderTest, ReorderingWorks3d_Test2)
    {
	MultiArray3dAny ma3d2(r3dc);
	auto i = ma3d2.begin();
	auto i1 = i.template getIndex<0>();
	auto i2 = i.template getIndex<1>();
	auto i3 = i.template getIndex<2>();

	ma3d2("gamma","alpha","beta") = ma3d("alpha","beta","gamma");
	
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 0,i3 = 0)],-5);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 0,i3 = 0)],6);
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 0,i3 = 1)],2);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 0,i3 = 1)],1);	
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 0,i3 = 2)],9);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 0,i3 = 2)],54);
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 0,i3 = 3)],27);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 0,i3 = 3)],-7);

	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 1,i3 = 0)],-13);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 1,i3 = 0)],32);
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 1,i3 = 1)],90);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 1,i3 = 1)],-67);
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 1,i3 = 2)],-10);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 1,i3 = 2)],16);
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 1,i3 = 3)],-2);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 1,i3 = 3)],101);

	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 2,i3 = 0)],39);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 2,i3 = 0)],-64);
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 2,i3 = 1)],81);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 2,i3 = 1)],-22);
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 2,i3 = 2)],14);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 2,i3 = 2)],34);
	EXPECT_EQ(ma3d2[i(i1 = 0,i2 = 2,i3 = 3)],95);
	EXPECT_EQ(ma3d2[i(i1 = 1,i2 = 2,i3 = 3)],-62);
    }
   
} // end namespace 

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
