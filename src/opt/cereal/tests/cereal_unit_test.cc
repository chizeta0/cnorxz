
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

    class CerURange_Test : public ::testing::Test
    {
    protected:

	CerURange_Test()
	{
	    mMeta = { "some", "random", "words", "with", "no", "meaning" };
	    mR = URangeFactory(mMeta).create();
	}

	Vector<String> mMeta;
	RangePtr mR;
	std::stringstream mS;
    };

    class CerYRange_Test : public ::testing::Test
    {
    protected:

	CerYRange_Test()
	{
	    mSize = 7;
	    mMeta = { "some", "random", "words", "with", "no", "meaning" };
	    mR1 = CRangeFactory(mSize).create();
	    mR2 = URangeFactory(mMeta).create();
	    mR = YRangeFactory({mR1,mR2}).create();
	}

	SizeT mSize;
	Vector<String> mMeta;
	RangePtr mR1;
	RangePtr mR2;
	RangePtr mR;
	std::stringstream mS;
	std::stringstream mS2;
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

    TEST_F(CerURange_Test, Serialize)
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

    TEST_F(CerYRange_Test, Serialize)
    {
	{
	    cereal::JSONOutputArchive ar(mS);
	    ar(cereal::make_nvp("mR",mR));
	    //ar(mR);
	}
	std::cout << mS.str() << std::endl;
	RangePtr r = nullptr;
	{
	    cereal::JSONInputArchive ar(mS);
	    cer::save_load(ar, "mR", r);
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

    TEST_F(CerYRange_Test, NoDuplicates)
    {
	{
	    cereal::JSONOutputArchive ar(mS);
	    ar(cereal::make_nvp("mR",mR));
	}
	{
	    cereal::JSONOutputArchive ar(mS2);
	    ar(cereal::make_nvp("mR",mR));
	}
	std::cout << mS.str() << std::endl;
	RangePtr r = nullptr;
	{
	    cereal::JSONInputArchive ar(mS);
	    //ar(r);
	    cer::save_load(ar, "mR", r);
	}
	RangePtr r2 = nullptr;
	{
	    cereal::JSONInputArchive ar(mS2);
	    //ar(r2);
	    cer::save_load(ar, "mR", r2);
	}

	EXPECT_TRUE(r == r2);
	EXPECT_EQ(r->size(), r2->size());
	EXPECT_EQ(r->dim(), r2->dim());
	for(SizeT i = 0; i != r->dim(); ++i){
	    EXPECT_TRUE(r->sub(i) == r2->sub(i));
	}
	auto i1 = r->begin();
	auto i2 = r2->begin();
	auto i1e = r->end();
	auto i2e = r2->end();
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
