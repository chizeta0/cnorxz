
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
	    cr = CRangeFactory(mSize).create();
	    auto crx = std::dynamic_pointer_cast<CRange>(cr);
	    ur = URangeFactory<String>(mMeta).create();
	    auto urx = std::dynamic_pointer_cast<URange<String>>(ur);
	    mr = mrange(crx,urx);
	    mr2 = mrange(crx,urx,crx);
	}

	Vector<String> mMeta;
	SizeT mSize;
	RangePtr mr;
	RangePtr mr2;
	RangePtr cr;
	RangePtr ur;
    };

    class YR_Test : public ::testing::Test
    {
    protected:

	YR_Test()
	{
	    mMeta = { "test", "strings", "foo" };
	    std::sort(mMeta.begin(), mMeta.end(), std::less<String>());
	    mSize = 7;
	    cr = CRangeFactory(mSize).create();
	    ur = URangeFactory<String>(mMeta).create();
	    yr = cr * ur;
	    yr2 = cr * ur * cr;
	}

	Vector<String> mMeta;
	SizeT mSize;
	RangePtr yr;
	RangePtr yr2;
	RangePtr cr;
	RangePtr ur;
    };
    
    TEST_F(CR_Test, Basics)
    {
	auto crx = std::dynamic_pointer_cast<CRange>(cr);
	EXPECT_FALSE(crx == nullptr);
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
	EXPECT_FALSE(urx == nullptr);
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

    TEST_F(MR_Test, Basics2d)
    {
	auto mrx = std::dynamic_pointer_cast<MRange<CRange,URange<String>>>(mr);
	EXPECT_FALSE(mrx == nullptr);

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
	    auto ci = xxi.pack()[CSizeT<0>{}];
	    auto ui = xxi.pack()[CSizeT<1>{}];
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

    TEST_F(MR_Test, Basics3d)
    {
	auto mrx = std::dynamic_pointer_cast<MRange<CRange,URange<String>,CRange>>(mr2);
	EXPECT_FALSE(mrx == nullptr);

	EXPECT_EQ(mr2->size(), mMeta.size()*mSize*mSize);
	EXPECT_EQ(mrx->size(), mMeta.size()*mSize*mSize);
	EXPECT_EQ(mr2->dim(), 3u);
	EXPECT_EQ(mrx->dim(), 3u);

	EXPECT_TRUE(mrx->begin() != mrx->end());
	EXPECT_FALSE(mrx->begin() == mrx->end());
	EXPECT_EQ(mrx->begin().pos(), 0u);
	EXPECT_EQ(mrx->end().pos(), mrx->size());

	EXPECT_TRUE(mr2->begin() != mr2->end());
	EXPECT_FALSE(mr2->begin() == mr2->end());
	EXPECT_EQ(mr2->begin().pos(), 0u);
	EXPECT_EQ(mr2->end().pos(), mr2->size());

	const SizeT s1 = mMeta.size();
	const SizeT s2 = mSize;
	const SizeT s1s2 = s1*s2;
	auto mkm = [&](SizeT i) { return Tuple<SizeT,String,SizeT>(i/s1s2,mMeta[(i/s2) % s1],i % s2); };
	
	SizeT cnt = 0;
	auto endxi = mr2->end();
	for(auto xi = mr2->begin(); xi != endxi; ++xi){
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
	    auto ci1 = xxi.pack()[CSizeT<0>{}];
	    auto ui = xxi.pack()[CSizeT<1>{}];
	    auto ci2 = xxi.pack()[CSizeT<2>{}];
	    Tuple<SizeT,String,SizeT> meta(*(*ci1),*(*ui),*(*ci2));
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

    TEST_F(YR_Test, Basics2d)
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

    TEST_F(YR_Test, Basics3d)
    {

	EXPECT_EQ(yr2->size(), mMeta.size()*mSize*mSize);
	EXPECT_EQ(yr2->dim(), 3u);
	
	EXPECT_TRUE(yr2->begin() != yr2->end());
	EXPECT_FALSE(yr2->begin() == yr2->end());
	EXPECT_EQ(yr2->begin().pos(), 0u);
	EXPECT_EQ(yr2->end().pos(), yr2->size());

	const SizeT s1 = mMeta.size();
	const SizeT s2 = mSize;
	const SizeT s1s2 = s1*s2;
	auto mkm = [&](SizeT i) { return Vector<DType>({DType(i/s1s2),DType(mMeta[(i/s2) % s1]),DType(i % s2)}); };

	SizeT cnt = 0;
	auto endxi = yr2->end();
	for(auto xi = yr2->begin(); xi != endxi; ++xi){
	    EXPECT_EQ(xi.pos(), cnt);
	    auto meta = mkm(cnt);
	    EXPECT_EQ(yr2->stringMeta(cnt), toString(meta));
	    EXPECT_TRUE(*xi == DType(meta));
	    EXPECT_EQ((*xi).str(), toString(meta));
	    ++cnt;
	}
    }

    TEST_F(YR_Test, Index2d)
    {
	auto yrx = std::dynamic_pointer_cast<YRange>(yr);
	EXPECT_FALSE(yrx == nullptr);
	auto yi = yrx->begin();

	EXPECT_EQ(yi.pmax().val(), yr->size());
	EXPECT_EQ(yi.lmax().val(), yr->size());
	EXPECT_EQ(yi.range(), yr);
	EXPECT_EQ(yi.range(), yrx);
	EXPECT_EQ(yi.dim(), 2u);
	
	const SizeT mmsize = mMeta.size();
	auto mkm = [&](SizeT i) { return Vector<DType>({DType(i/mmsize),DType(mMeta[i % mmsize])}); };
	for(SizeT i = 0; i != yr->size(); ++i){
	    auto a = yi + i;
	    EXPECT_EQ(a.lex(), i);
	    EXPECT_EQ(a.pos(), i);
	    auto mmi = mkm(i);
	    EXPECT_TRUE(a.meta() == mmi);
	    EXPECT_TRUE(*a == mmi);
	    EXPECT_EQ(a.stringMeta(), toString(mmi));
	    
	    for(SizeT j = 0; j != yr->size(); ++j){
		const Int jj = static_cast<Int>(j) - static_cast<Int>(i);
		auto b = a + jj;
		auto mmj = mkm(j);
		EXPECT_EQ(b.lex(), j);
		EXPECT_EQ(b.pos(), j);
		EXPECT_TRUE(*b == mmj);
		EXPECT_EQ(b.stringMeta(), toString(mmj));
	    }
	}
	yi += yi.lmax().val() + 10;
	EXPECT_EQ(yi.lex(), yi.lmax().val());
	EXPECT_EQ(yi.pos(), yi.pmax().val());

	yi -= yi.lmax().val() + 20;
	EXPECT_EQ(yi.lex(), 0u);
	EXPECT_EQ(yi.pos(), 0u);
    }

    TEST_F(YR_Test, Index3d)
    {
	auto yrx = std::dynamic_pointer_cast<YRange>(yr2);
	EXPECT_FALSE(yrx == nullptr);
	auto yi = yrx->begin();

	EXPECT_EQ(yi.pmax().val(), yr2->size());
	EXPECT_EQ(yi.lmax().val(), yr2->size());
	EXPECT_EQ(yi.range(), yr2);
	EXPECT_EQ(yi.range(), yrx);
	EXPECT_EQ(yi.dim(), 3u);
	
	const SizeT s1 = mMeta.size();
	const SizeT s2 = mSize;
	const SizeT s1s2 = s1*s2;
	auto mkm = [&](SizeT i) { return Vector<DType>({DType(i/s1s2),DType(mMeta[(i/s2) % s1]),DType(i % s2)}); };
	for(SizeT i = 0; i != yr2->size(); ++i){
	    auto a = yi + i;
	    EXPECT_EQ(a.lex(), i);
	    EXPECT_EQ(a.pos(), i);
	    auto mmi = mkm(i);
	    EXPECT_TRUE(a.meta() == mmi);
	    EXPECT_TRUE(*a == mmi);
	    EXPECT_EQ(a.stringMeta(), toString(mmi));
	    
	    for(SizeT j = 0; j != yr2->size(); ++j){
		const Int jj = static_cast<Int>(j) - static_cast<Int>(i);
		auto b = a + jj;
		auto mmj = mkm(j);
		EXPECT_EQ(b.lex(), j);
		EXPECT_EQ(b.pos(), j);
		EXPECT_TRUE(*b == mmj);
		EXPECT_EQ(b.stringMeta(), toString(mmj));
	    }
	}
	yi += yi.lmax().val() + 10;
	EXPECT_EQ(yi.lex(), yi.lmax().val());
	EXPECT_EQ(yi.pos(), yi.pmax().val());

	yi -= yi.lmax().val() + 20;
	EXPECT_EQ(yi.lex(), 0u);
	EXPECT_EQ(yi.pos(), 0u);
    }

    TEST_F(YR_Test, IndexMultiplication)
    {
	const SizeT s1 = mMeta.size();
	const SizeT s2 = mSize;
	auto crx = std::dynamic_pointer_cast<CRange>(cr);
	auto urx = std::dynamic_pointer_cast<URange<String>>(ur);
	for(auto cix = crx->begin(); cix != crx->end(); ++cix){
	    for(auto uix = urx->begin(); uix != urx->end(); ++uix){
		const SizeT p1 = cix.lex()*s1 + uix.lex();
		EXPECT_EQ(mindex(cix*uix).lex(), p1);
		for(auto ci2x = crx->begin(); ci2x != crx->end(); ++ci2x){
		    const SizeT p2 = cix.lex()*s1*s2 + uix.lex()*s2 + ci2x.lex();
		    EXPECT_EQ(mindex(cix*uix*ci2x).lex(), p2);
		}
	    }
	}
	for(auto ci = cr->begin(); ci != cr->end(); ++ci){
	    for(auto ui = ur->begin(); ui != ur->end(); ++ui){
		const SizeT p = ci.lex()*s1 + ui.lex();
		EXPECT_EQ(yindex(ci*ui).lex(), p);
		for(auto ci2 = cr->begin(); ci2 != cr->end(); ++ci2){
		    const SizeT p2 = ci.lex()*s1*s2 + ui.lex()*s2 + ci2.lex();
		    EXPECT_EQ(yindex(ci*ui*ci2).lex(), p2);
		}
	    }
	}
    }
    
// RCast_Test
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
