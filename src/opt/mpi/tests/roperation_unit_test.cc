// -*- C++ -*-
/**
   
   @file opt/mpi/tests/roperation_unit_test.cc
   @brief Rank operation unit tests.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#include <cstdlib>
#include <iostream>

#include "gtest/gtest.h"

#include "cnorxz.h"
#include "cnorxz_mpi.h"
#include "test_numbers.h"
#include "rrange.cc.h"
#include "rarray.cc.h"
#include "mpi_env.h"

namespace
{
    using namespace CNORXZ;
    using Test::Numbers;
    using namespace CNORXZ::mpi;

    class ROp_Test : public ::testing::Test
    {
    protected:

	ROp_Test()
	{
	    CXZ_ASSERT(getNumRanks() == 4, "exptected 4 ranks");
	    const SizeT myrank = getRankNumber();
	    T = 16;
	    L = 12;
	    mTRange = CRangeFactory(T).create();
	    mLRange = CRangeFactory(L).create();
	    mSpRange = SRangeFactory(Arr<SizeT,4>{0,1,2,3}).create();
	    mXRange = mTRange*mLRange*mLRange*mLRange;
	    
	    RangePtr g1 = CRangeFactory(1).create();
	    RangePtr g2 = CRangeFactory(2).create();
	    mGeom = g2*g1*g1*g2*g1;
	    mGeom2 = g2*g1*g1*g2;
	    
	    mRXRange = rrange(mXRange, g2*g1*g1*g2);
	    RangePtr scr = mSpRange*mSpRange;
	    const Vector<Double> vec = Numbers::get(0,mRXRange->sub(1)->size()+2);
	    RangePtr ltr = mRXRange->sub(1)->sub(0);
	    RangePtr ll1r = mRXRange->sub(1)->sub(1);
	    RangePtr ll2r = mRXRange->sub(1)->sub(2);
	    RangePtr ll3r = mRXRange->sub(1)->sub(3);
	    mMRange = ltr*ll1r*ll2r*ll3r*scr;
	    Vector<Double> data(mMRange->size());
	    Vector<Double> data2(mMRange->size());
	    Double xxx = 1;
	    for(SizeT i = 0; i != mRXRange->sub(1)->size(); ++i){
		for(SizeT j = 0; j != scr->size(); ++j){
		    const SizeT k = i*scr->size() + j; 
		    data[k] = vec[i] * static_cast<Double>(j+2) + (xxx += 3*mpi::getRankNumber());
		    data2[k] = vec[i] / static_cast<Double>(j+2) + (xxx += 5*mpi::getRankNumber());
		    if(k > 0){ 
			assert(data[k] != data[k-1]);
		    }
		}
	    }
	    mM1 = RCArray<Double>( MArray<Double>(mMRange, data), mGeom );
	    mM2 = RCArray<Double>( MArray<Double>(mMRange, data2), mGeom );
	    mAll1 = Vector<Double>(data.size() * getNumRanks());
	    mAll2 = Vector<Double>(data2.size() * getNumRanks());
	    typedef RIndex<MIndex<CIndex,CIndex,CIndex,CIndex>,MIndex<CIndex,CIndex,CIndex,CIndex>> RI;
	    const SizeT scrs = scr->size();
	    auto rix = RI(mRXRange);
	    assert(rix.lmax().val() == 27648);
	    assert(scrs == 16);
	    for(auto ri = RI(mRXRange); ri.lex() != ri.lmax().val(); ++ri){
		Double* buf = mAll1.data() + scrs*ri.lex();
		Double* buf2 = mAll2.data() + scrs*ri.lex();
		if(ri.rank() == myrank){
		    std::memcpy(buf, data.data()+ri.local()->lex()*scrs, scrs*sizeof(Double));
		    std::memcpy(buf2, data2.data()+ri.local()->lex()*scrs, scrs*sizeof(Double));
		}
		MPI_Bcast(buf, scrs, MPI_DOUBLE, ri.rank(), MPI_COMM_WORLD);
		MPI_Bcast(buf2, scrs, MPI_DOUBLE, ri.rank(), MPI_COMM_WORLD);
	    }
	}

	SizeT T;
	SizeT L;
	RangePtr mTRange;
	RangePtr mLRange;
	RangePtr mXRange; // global T*L*L*L
	RangePtr mRXRange;
	RangePtr mSpRange;
	RangePtr mMRange;
	RangePtr mGeom;
	RangePtr mGeom2;

	RCArray<Double> mM1;
	RCArray<Double> mM2;
	Vector<Double> mAll1;
	Vector<Double> mAll2;
    };

    TEST_F(ROp_Test, Check)
    {
	EXPECT_EQ(mM1.size(), mM2.size());
	EXPECT_EQ(mM1.size(), mAll1.size());
	for(auto i = mM1.begin(); i.lex() != i.lmax().val(); ++i){
	    const auto a1 = *i;
	    const auto a2 = mAll1[i.lex()];
	    EXPECT_EQ(a1, a2);
	}
    }
    
    TEST_F(ROp_Test, Difference)
    {
	RArray<Double> res( MArray<Double>(mM1.range()->sub(1)), mGeom );
	Vector<Double> comp( mXRange->size()*mSpRange->size()*mSpRange->size() );
	EXPECT_EQ(res.size(), comp.size());

	typedef UIndex<SizeT> UI;
	
	auto xp = std::make_shared<RIndex<MIndex<UI,UI,UI,UI>,MIndex<UI,UI,UI,UI>>>(mRXRange);
	auto xm = std::make_shared<RIndex<MIndex<UI,UI,UI,UI>,MIndex<UI,UI,UI,UI>>>(mRXRange);
	auto x = std::make_shared<RIndex<MIndex<UI,UI,UI,UI>,MIndex<UI,UI,UI,UI>>>(mRXRange);
	auto A = std::make_shared<SIndex<SizeT,4>>(mSpRange);
	auto B = std::make_shared<SIndex<SizeT,4>>(mSpRange);
	auto AB = mindexPtr(A*B);
	
	Sptr<Vector<SizeT>> imap1;
	imap1 = setupMap(xp, x, [&](const auto& vec) {
	    return std::make_tuple((std::get<0>(vec)+1)%T, (std::get<1>(vec)+1)%L,
				   (std::get<2>(vec)+1)%L, (std::get<3>(vec)+1)%L); } );
	Vector<bool> req(xp->range()->size(), false);
	for(const auto& r: *imap1){
	    req[(r+mpi::getRankNumber()*16*12*12*12/4)%req.size()] = true;
	}
	res.load(x, AB, req); // DUMMY, not used...
	mM1.load(xp, AB, req);
	//res.rop(x*A*B) = mapXpr(xp,x,imap1, mM1(xp*A*B) - mM1(x*A*B) );
	res(x*A*B) = mapXpr(xp,x,imap1, mM1(xp*A*B) - mM1(x*A*B) );

	for(SizeT x0 = 0; x0 != T; ++x0) { 
	for(SizeT x1 = 0; x1 != L; ++x1)
	for(SizeT x2 = 0; x2 != L; ++x2)
	for(SizeT x3 = 0; x3 != L; ++x3)
	for(SizeT A = 0; A != 4; ++A)
	for(SizeT B = 0; B != 4; ++B) {
	    const SizeT xi = x0*L*L*L + x1*L*L + x2*L + x3;
	    const SizeT x0p = (x0+1)%T;
	    const SizeT x1p = (x1+1)%L;
	    const SizeT x2p = (x2+1)%L;
	    const SizeT x3p = (x3+1)%L;
	    const SizeT xpi = x0p*L*L*L + x1p*L*L + x2p*L + x3p;
	    const SizeT pi = xpi*4*4 + A*4 + B;
	    const SizeT ri = xi*4*4 + A*4 + B;
	    comp[ri] = mAll1[pi] - mAll1[ri];
	}}
	
	for(auto i = res.begin(); i.lex() != i.lmax().val(); ++i){
	    const auto a1 = *i;
	    const auto a2 = comp[i.lex()];
	    EXPECT_EQ(a1, a2);
	}
    }
    
    TEST_F(ROp_Test, Contract)
    {
	Vector<Double> comp(mRXRange->size());
	RArray<Double> res( MArray<Double>(mRXRange->sub(1)), mGeom2 );
	EXPECT_EQ(res.size(), comp.size());
	typedef UIndex<SizeT> UI;
	auto xl = std::make_shared<RIndex<MIndex<UI,UI,UI,UI>,MIndex<UI,UI,UI,UI>>>(mRXRange);
	auto y = std::make_shared<RIndex<MIndex<UI,UI,UI,UI>,MIndex<UI,UI,UI,UI>>>(mRXRange);
	auto xy = std::make_shared<RIndex<MIndex<UI,UI,UI,UI>,MIndex<UI,UI,UI,UI>>>(mRXRange);
	auto x = std::make_shared<RIndex<MIndex<UI,UI,UI,UI>,MIndex<UI,UI,UI,UI>>>(mRXRange);
	auto A = std::make_shared<SIndex<SizeT,4>>(mSpRange);
	auto B = std::make_shared<SIndex<SizeT,4>>(mSpRange);

	Sptr<Vector<SizeT>> imap1;
	Sptr<Vector<SizeT>> imap2;
	auto AB = mindexPtr(A*B);
	auto BA = mindexPtr(B*A);
	auto myx = mindexPtr(y*xl);
	
	// block 1:
	//imap1 = setupMap(x, xl, [](const auto& vec) { return vec; } );
	//imap2 = setupMap(xy, myx, [](const auto& vec) { return std::get<0>(vec); } );
	//mM1.load(xl, x, AB, imap1);
	//mM2.load(myx, xy, BA, imap2);
	//res(y).a(mindexPtr(y*xl), [](auto& t, const auto& s) { t += s; },
	//	 mapXpr( x, xl, imap1, mapXpr( xy, myx, imap2, (mM1(x*A*B) * mM2(xy*B*A)).c(AB) ) ) );
	/*
	// block 2:
	mM1.load(,A*B*a*b);
	mM2.load(,A*B*a*b);
	res(y) += (mM1(x*A*B*a*b) * mM2(xy*B*A*b*a)).c(mindexPtr(x*A*B));
	// block 3:
	mM1.load(,A*B*a*b);
	mM2.load(,A*B*a*b);
	res(y) += (mM1(x*A*B*a*b) * mM2(xy*B*A*b*a)).c(x*A*B*a*b);
	// block 4:
	mM1.load(,A*B*a*b);
	mM2.load(,A*B*a*b);
	res(y) += (mM1(x*A*B*a*b) * mM2(xy*B*A*b*a)).c(x*A*B*a*b);
	// comparison loop
	for(SizeT x0 = 0; x0 != T; ++x0) { VCHECK(x0);
	for(SizeT x1 = 0; x1 != L; ++x1)
	for(SizeT x2 = 0; x2 != L; ++x2)
	for(SizeT x3 = 0; x3 != L; ++x3)
	for(SizeT y0 = 0; y0 != T; ++y0)
	for(SizeT y1 = 0; y1 != L; ++y1)
	for(SizeT y2 = 0; y2 != L; ++y2)
	for(SizeT y3 = 0; y3 != L; ++y3)
	for(SizeT A = 0; A != 4; ++A)
	for(SizeT B = 0; B != 4; ++B) {
	    const SizeT yi = y0*L*L*L + y1*L*L + y2*L + y3;
	    const SizeT xi = x0*L*L*L + x1*L*L + x2*L + x3;
	    const SizeT xy0 = (x0 + y0) % T;
	    const SizeT xy1 = (x1 + y1) % L;
	    const SizeT xy2 = (x2 + y2) % L;
	    const SizeT xy3 = (x3 + y3) % L;
	    const SizeT xyi = xy0*L*L*L + xy1*L*L + xy2*L + xy3;
	    const SizeT i1 = xi*4*4 + A*4 + B;
	    const SizeT i2 = xyi*4*4 + B*4 + A;
	    comp[yi] += mAll1[i1] * mAll2[i2];
	}}
	VCHECK(comp[123]);
	*/
	/*
	for(auto i = res.begin(); i.lex() != i.lmax().val(); ++i){
	    EXPECT_EQ(*i, comp[i.lex()]);
	}
	*/
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment( new MPIEnv(argc, argv) );
    return RUN_ALL_TESTS();
}
