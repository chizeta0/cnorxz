
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
	    cr = CRangeFactory(mSize).create();
	}

	SizeT mSize;
	RangePtr cr;
    };

    class UR_Test : public ::testing::Test
    {
    protected:

	UR_Test()
	{
	    mMeta = { "These", "are", "test", "strings", "foo", "bar", "baz" };
	    std::sort(mMeta.begin(), mMeta.end(), std::less<String>());
	    ur = URangeFactory<String>(mMeta).create();
	}

	Vector<String> mMeta;
	RangePtr ur;
    };

    class MR_Test : public ::testing::Test
    {
    protected:

	MR_Test()
	{
	    mMeta = { "test", "strings", "foo" };
	    std::sort(mMeta.begin(), mMeta.end(), std::less<String>());
	    mSize = 7;
	    auto cr = CRangeFactory(mSize).create();
	    auto crx = std::dynamic_pointer_cast<CRange>(cr);
	    auto ur = URangeFactory<String>(mMeta).create();
	    auto urx = std::dynamic_pointer_cast<URange<String>>(ur);
	    mr = mrange(crx,urx);
	}

	Vector<String> mMeta;
	SizeT mSize;
	RangePtr mr;
    };

    class YR_Test : public ::testing::Test
    {
    protected:

	YR_Test()
	{
	    mMeta = { "test", "strings", "foo" };
	    std::sort(mMeta.begin(), mMeta.end(), std::less<String>());
	    mSize = 7;
	    auto cr = CRangeFactory(mSize).create();
	    auto ur = URangeFactory<String>(mMeta).create();
	    yr = cr * ur;
	}

	Vector<String> mMeta;
	SizeT mSize;
	RangePtr yr;
    };
    
    TEST_F(CR_Test, Basics)
    {
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
	for(const auto& x: *urx){
	    EXPECT_EQ(x, mMeta[cnt2++]);
	}
    }

    TEST_F(MR_Test, Basics)
    {
	auto mrx = std::dynamic_pointer_cast<MRange<CRange,URange<String>>>(mr);

	EXPECT_EQ(mr->size(), mMeta.size()*mSize);
	EXPECT_EQ(mrx->size(), mMeta.size()*mSize);
	EXPECT_EQ(mr->dim(), 2u);
	EXPECT_EQ(mrx->dim(), 2u);

	EXPECT_TRUE(mrx->begin() != mrx->end());
	EXPECT_FALSE(mrx->begin() == mrx->end());
	EXPECT_EQ(mrx->begin().pos(), 0u);
	EXPECT_EQ(mrx->end().pos(), mrx->size());

	EXPECT_TRUE(mr->begin() != mr->end());
	EXPECT_FALSE(mr->begin() == mr->end());
	EXPECT_EQ(mr->begin().pos(), 0u);
	EXPECT_EQ(mr->end().pos(), mr->size());

	const SizeT mmsize = mMeta.size();
	auto mkm = [&](SizeT i) { return Tuple<SizeT,String>(i/mmsize,mMeta[i % mmsize]); };
	
	SizeT cnt = 0;
	auto endxi = mr->end();
	for(auto xi = mr->begin(); xi != endxi; ++xi){
	    EXPECT_EQ(xi.pos(), cnt);
	    EXPECT_EQ(xi.lex(), cnt);
	    auto meta = mkm(cnt);
	    EXPECT_TRUE(*xi == DType(meta));
	    EXPECT_EQ((*xi).str(), toString(meta));
	    ++cnt;
	}

	cnt = 0;
	auto endxxi = mrx->end();
	for(auto xxi = mrx->begin(); xxi != endxxi; ++xxi){
	    EXPECT_EQ(xxi.pos(), cnt);
	    EXPECT_EQ(xxi.lex(), cnt);
	    auto ci = std::get<0>(xxi.pack());
	    auto ui = std::get<1>(xxi.pack());
	    Tuple<SizeT,String> meta(*(*ci),*(*ui));
	    auto meta2 = mkm(cnt);
	    EXPECT_EQ(meta, meta2);
	    EXPECT_TRUE(*xxi == meta);
	    ++cnt;
	}

	cnt = 0;
	for(auto x: *mrx){
	    auto meta = mkm(cnt);
	    EXPECT_EQ(x, meta);
	    ++cnt;
	}
    }

    TEST_F(YR_Test, Basics)
    {

	EXPECT_EQ(yr->size(), mMeta.size()*mSize);
	EXPECT_EQ(yr->dim(), 2u);
	
	EXPECT_TRUE(yr->begin() != yr->end());
	EXPECT_FALSE(yr->begin() == yr->end());
	EXPECT_EQ(yr->begin().pos(), 0u);
	EXPECT_EQ(yr->end().pos(), yr->size());

	const SizeT mmsize = mMeta.size();
	auto mkm = [&](SizeT i) { return Vector<DType>({DType(i/mmsize),DType(mMeta[i % mmsize])}); };

	SizeT cnt = 0;
	auto endxi = yr->end();
	for(auto xi = yr->begin(); xi != endxi; ++xi){
	    EXPECT_EQ(xi.pos(), cnt);
	    auto meta = mkm(cnt);
	    EXPECT_TRUE(*xi == DType(meta));
	    EXPECT_EQ(yr->stringMeta(cnt), toString(meta));
	    EXPECT_EQ((*xi).str(), toString(meta));
	    ++cnt;
	}
    }

    TEST_F(YR_Test, Index)
    {
	auto yrx = std::dynamic_pointer_cast<YRange>(yr);
	auto yi = yrx->begin();

	EXPECT_EQ(yi.pmax(), yr->size());
	EXPECT_EQ(yi.lmax(), yr->size());
	EXPECT_EQ(yi.range(), yr);
	EXPECT_EQ(yi.range(), yrx);
	EXPECT_EQ(yi.dim(), 2u);
	
	const SizeT mmsize = mMeta.size();
	auto mkm = [&](SizeT i) { return Vector<DType>({DType(i/mmsize),DType(mMeta[i % mmsize])}); };
	for(SizeT i = 0; i != yr->size(); ++i){
	    auto a = yi + i;
	    EXPECT_EQ(a.lex(), i);
	    EXPECT_EQ(a.pos(), i);
	    auto mmi = DType(mkm(i));
	    EXPECT_TRUE(a.meta() == mmi);
	    EXPECT_TRUE(*a == mmi);
	    EXPECT_EQ(a.stringMeta(), toString(mmi));
	    
	    for(SizeT j = 0; j != yr->size(); ++j){
		const Int jj = static_cast<Int>(j) - static_cast<Int>(i);
		auto b = a + jj;
		auto mmj = DType(mkm(j));
		EXPECT_EQ(b.lex(), j);
		EXPECT_EQ(b.pos(), j);
		EXPECT_TRUE(*b == mmj);
		EXPECT_EQ(b.stringMeta(), toString(mmj));
	    }
	}
	yi += yi.lmax() + 10;
	EXPECT_EQ(yi.lex(), yi.lmax());
	EXPECT_EQ(yi.pos(), yi.pmax());

	yi -= yi.lmax() + 20;
	EXPECT_EQ(yi.lex(), 0u);
	EXPECT_EQ(yi.pos(), 0u);
    }
    // RCast_Test
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
