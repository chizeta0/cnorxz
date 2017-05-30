// -*- C++ -*-

#include <cstdlib>
#include "gtest/gtest.h"
#include <iostream>

#include "multi_array_header.h"

namespace MAT = MultiArrayTools;

namespace {

    class ConnectionTest : public ::testing::Test
    {
    protected:

	typedef MAT::SingleRange<char,MAT::RangeType::ANY> SingleRc;
	typedef MAT::SingleRange<int,MAT::RangeType::ANY> SingleRi;
	typedef MAT::MultiRange<SingleRc,SingleRc,SingleRi> MultiR3d;
	typedef MAT::MultiRange<SingleRi,SingleRc> MultiR2d;

	ConnectionTest() : sri1({0,1,2,3}),
			   src1({'a','b'}),
			   src2({'a','b','c'}),
			   mr3d1(src1,src2,sri1) {}
	
	SingleRi sri1;
	SingleRc src1;
	SingleRc src2;
	MultiR3d mr3d1;
    };

    TEST_F(ConnectionTest, UnlinkedIteration)
    {
	auto i3d = mr3d1.begin();
	auto& ii1 = i3d.template getIndex<2>();
	EXPECT_EQ(i3d.pos(), 0);
	++ii1;
	EXPECT_EQ(i3d.pos(), 1);
	++ii1;
	EXPECT_EQ(i3d.pos(), 2);
	++ii1;
	EXPECT_EQ(i3d.pos(), 3);
    }
    
} // end namespace 

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
