
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "gtest/gtest.h"

#include "cnorxz.h"
#include "cnorxz_cereal.h"
#include <cereal/archives/json.hpp>

namespace
{
    using namespace CNORXZ;

    class CerCRange_Test : public ::testing::Test
    {
    protected:

	CerCRange_Test()
	{
	    mSize = 7;
	    mR = CRangeFactory(mSize).create();
	}

	SizeT mSize;
	RangePtr mR;
	std::stringstream mS;
    };

    TEST_F(CerCRange_Test, Serialize)
    {
	{
	    cereal::JSONOutputArchive ar(mS);
	    ar(mR);
	}
	std::cout << mS.str() << std::endl;
	RangePtr r = nullptr;
	{
	    cereal::JSONInputArchive ar(mS);
	    ar(r);
	}
	EXPECT_EQ(r->size(), mR->size());
	EXPECT_EQ(r->dim(), mR->dim());
	auto i1 = r->begin();
	auto i2 = mR->begin();
	auto i1e = r->end();
	auto i2e = mR->end();
	for(; i1 != i1e; ++i1, ++i2){
	    EXPECT_TRUE(*i1 == *i2);
	}
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
