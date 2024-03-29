// -*- C++ -*-
/**
   
   @file tests/range_unit_test.cc
   @brief Range unit tests.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#include <cstdlib>
#include <iostream>

#include "gtest/gtest.h"

#include "ranges/ranges.h"
#include "array/array.h"

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

    TEST_F(CR_Test, prange)
    {
	auto crx = std::dynamic_pointer_cast<CRange>(cr);
	auto beg = crx->begin();
	auto end = crx->end();

	const SizeT begPos = 1;
	const SizeT endPos = mSize-2;
	const SizeT pSize = endPos - begPos;
	beg += begPos;
	end -= mSize-endPos+1;
	auto pr = beg.prange(end);
	auto prx = std::dynamic_pointer_cast<PRange<CRange>>(pr);

	EXPECT_EQ(pr->size(), pSize);
	EXPECT_EQ(prx->size(), pSize);

	for(auto i = prx->begin(); i != prx->end(); ++i){
	    EXPECT_EQ(*i, i.lex()+begPos);
	}

	EXPECT_EQ(prx->orig(),cr);
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

    TEST_F(UR_Test, prange)
    {
	auto urx = std::dynamic_pointer_cast<URange<String>>(ur);
	auto beg = urx->begin();
	auto end = urx->end();

	const SizeT begPos = 1;
	const SizeT endPos = mMeta.size() - 2;
	const SizeT pSize = endPos - begPos;
	beg += begPos;
	end -= mMeta.size() - endPos + 1;
	auto pr = beg.prange(end);
	auto prx = std::dynamic_pointer_cast<PRange<URange<String>>>(pr);

	EXPECT_EQ(pr->size(), pSize);
	EXPECT_EQ(prx->size(), pSize);

	for(auto i = prx->begin(); i != prx->end(); ++i){
	    EXPECT_EQ(*i, mMeta[i.lex()+begPos]);
	}
	EXPECT_EQ(prx->orig(),ur);
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

    TEST_F(YR_Test, prange)
    {
	auto yrx = std::dynamic_pointer_cast<YRange>(yr);
	auto beg = yrx->begin();
	auto last = yrx->end();

	const SizeT begI = 1;
	const SizeT lastI = mSize - 2 - 1;
	const SizeT begJ = 1;
	const SizeT lastJ = mMeta.size() - 1;

	const SizeT begPos = begI * mMeta.size() + begJ;
	const SizeT lastPos = lastI * mMeta.size() + lastJ;
	const SizeT pSize = (lastI-begI+1)*(lastJ-begJ+1);
	beg += begPos;
	last = lastPos;
	auto pr = beg.prange(last);
	auto prx = std::dynamic_pointer_cast<YRange>(pr);

	EXPECT_EQ(pr->size(), pSize);
	EXPECT_EQ(prx->size(), pSize);

	const SizeT mmsize = lastJ-begJ+1;
	auto mkm = [&](SizeT i) {
	    return Vector<DType>({DType(i/mmsize+begI),DType(mMeta[i % mmsize+begJ])}); };

	for(auto i = prx->begin(); i != prx->end(); ++i){
	    EXPECT_TRUE(*i == mkm(i.lex()));
	}

	const SizeT begI2 = 1;
	const SizeT lastI2 = 3;
	const SizeT begJ2 = 0;
	const SizeT lastJ2 = 0;

	const SizeT begPos2 = begI2 * (lastJ-begJ+1) + begJ2;
	const SizeT lastPos2 = lastI2 * (lastJ-begJ+1) + lastJ2;
	const SizeT pSize2 = (lastI2-begI2+1)*(lastJ2-begJ2+1);

	auto beg2 = prx->begin();
	auto last2 = prx->begin();

	beg2 = begPos2;
	last2 = lastPos2;
	auto pr2 = beg2.prange(last2);
	auto prx2 = std::dynamic_pointer_cast<YRange>(pr2);

	EXPECT_EQ(pr2->size(), pSize2);
	EXPECT_EQ(prx2->size(), pSize2);
	
	const SizeT mmsize2 = lastJ2-begJ2+1;
	auto mkm2 = [&](SizeT i) {
	    return Vector<DType>({DType(i/mmsize2+begI2+begI),DType(mMeta[i % mmsize2+begJ2+begJ])}); };

	for(auto i = prx2->begin(); i != prx2->end(); ++i){
	    EXPECT_TRUE(*i == mkm2(i.lex()));
	}
	
    }

// RCast_Test
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
