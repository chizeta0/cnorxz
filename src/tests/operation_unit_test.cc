
#include <cstdlib>
#include <iostream>

#include "gtest/gtest.h"

#include "ranges/ranges.h"
#include "operation/operation.h"
#include "test_numbers.h"

namespace
{
    using namespace CNORXZ;
    using Test::Numbers;

    class OpCont_CR_Test : public ::testing::Test
    {
    protected:

	OpCont_CR_Test()
	{
	    mSize = 7;
	    const SizeT off = 10;
	    mData1 = Numbers::get(off, mSize);
	    mData2 = Numbers::get(off + mSize*2, mSize);
	    auto cr = CRangeFactory(mSize).create();
	    mCI1 = std::make_shared<CIndex>(cr);
	    mCI2 = std::make_shared<CIndex>(cr);
	    mOp1.init(mCI1);
	    mOp2.init(mData1.data(), mCI1);
	    mOp3.init(mCI2);
	    mOp4.init(mData2.data(), mCI2);
	}

	SizeT mSize;
	Vector<Double> mData1;
	Vector<Double> mData2;
	Sptr<CIndex> mCI1;
	Sptr<CIndex> mCI2;
	OpCont<double,CIndex> mOp1;
	COpRoot<double,CIndex> mOp2;
	OpCont<double,CIndex> mOp3;
	COpRoot<double,CIndex> mOp4;
    };

    class OpCont_CR_CR_Test : public ::testing::Test
    {
    protected:

	typedef MIndex<CIndex,CIndex> MCCI;
	
	OpCont_CR_CR_Test()
	{
	    mSize1 = 7;
	    mSize2 = 11;
	    SizeT off = 20;
	    mData1 = Numbers::get(off, mSize1);
	    mData2 = Numbers::get(off += mSize1 , mSize2);
	    mData11 = Numbers::get(off += mSize2, mSize1*mSize2);
	    mData12 = Numbers::get(off += mSize1*mSize2, mSize1*mSize2);
	    auto cr1 = CRangeFactory(mSize1).create();
	    auto cr2 = CRangeFactory(mSize2).create();
	    mCI1i = std::make_shared<CIndex>(cr1);
	    mCI1j = std::make_shared<CIndex>(cr1);
	    mCI2i = std::make_shared<CIndex>(cr2);
	    mCI2j = std::make_shared<CIndex>(cr2);
	    mCC1i1j = mindexPtr(mCI1i*mCI1j);
	    mCC1j1i = mindexPtr(mCI1j*mCI1i);
	    //mCC1i2i = mindexPtr(mCI1i*mCI2i);
	    //mCC1j2i = mindexPtr(mCI1j*mCI2i);
	    mOC1i1j.init(mCC1i1j);
	    mOR1j1i.init(mData11.data(), mCC1j1i);
	    mOR1i1j.init(mData12.data(), mCC1i1j);
	}

	SizeT mSize1;
	SizeT mSize2;
	Vector<Double> mData1;
	Vector<Double> mData2;
	Vector<Double> mData11;
	Vector<Double> mData12;
	Sptr<CIndex> mCI1i;
	Sptr<CIndex> mCI1j;
	Sptr<CIndex> mCI2i;
	Sptr<CIndex> mCI2j;
	Sptr<MCCI> mCC1i1j;
	Sptr<MCCI> mCC1j1i;
	//Sptr<MCCI> mCC1i2i;
	//Sptr<MCCI> mCC1j2i;
	OpCont<double,MCCI> mOC1i1j;
	COpRoot<double,MCCI> mOR1j1i;
	COpRoot<double,MCCI> mOR1i1j;
	//COpRoot<double,MCCI> mOR1j2i;
	//COpRoot<double,MCCI> mOR1i2i;
    };

    class OpCont_CR_CR_Test2 : public ::testing::Test
    {
    protected:

	typedef MIndex<CIndex,LIndex<CIndex,2>,EIndex<SizeT,4,1>> MCCI1;
	typedef GMIndex<GMFormat<UPos,UPos,SPos<1>>,CIndex,LIndex<CIndex,2>,EIndex<SizeT,4,1>> MCCI1x;
	typedef MIndex<LIndex<CIndex,2>,EIndex<SizeT,4,1>,CIndex> MCCI2;
	
	OpCont_CR_CR_Test2()
	{
	    mSize1 = 11;
	    mSize2 = 12;
	    SizeT off = 20;
	    //mData1 = Numbers::get(off, mSize1);
	    //mData2 = Numbers::get(off += mSize1 , mSize2);
	    mData21 = Numbers::get(off += mSize2, mSize1*mSize2);
	    mData12 = Numbers::get(off += mSize1*mSize2, mSize1*mSize2);
	    auto cr1 = CRangeFactory(mSize1).create();
	    auto cr2 = CRangeFactory(mSize2).create();
	    mCI1i = std::make_shared<CIndex>(cr1);
	    mCI1j = std::make_shared<CIndex>(cr1);
	    mCI2i = std::make_shared<CIndex>(cr2);
	    mCI2j = std::make_shared<CIndex>(cr2);
	    auto ipack1 = mCI1i*eplex<4,1,2>(mCI2j);
	    auto iform1 = gmformat(UPos(mCI2j->lmax().val()),UPos(4),SPos<1>());
	    mCC1i2j = gmindexPtr( iform1, ipack1 );
	    //mCC1i2j = mindexPtr(mCI1i*eplex<4,1,2>(mCI2j));
	    mCC2j1i = mindexPtr(eplex<4,1,2>(mCI2j)*mCI1i);
	    mOC1i2j.init(mCC1i2j);
	    mOR2j1i.init(mData21.data(), mCC2j1i);
	    mOR1i2j.init(mData12.data(), mCC1i2j);
	}

	SizeT mSize1;
	SizeT mSize2;
	Vector<Double> mData12;
	Vector<Double> mData21;
	Vector<Double> mData11_1;
	Vector<Double> mData11_2;
	Sptr<CIndex> mCI1i;
	Sptr<CIndex> mCI1j;
	Sptr<CIndex> mCI2i;
	Sptr<CIndex> mCI2j;
	Sptr<MCCI1x> mCC1i2j;
	Sptr<MCCI2> mCC2j1i;
	OpCont<double,MCCI1x> mOC1i2j;
	COpRoot<double,MCCI2> mOR2j1i;
	COpRoot<double,MCCI1x> mOR1i2j;
    };

    TEST_F(OpCont_CR_Test, Basics)
    {
	EXPECT_EQ(mOp2.data(), mData1.data());
	EXPECT_EQ(mOp4.data(), mData2.data());
	auto rs11 = mOp1.rootSteps(mCI1->id());
	auto rs21 = mOp2.rootSteps(mCI1->id());
	auto rs31 = mOp3.rootSteps(mCI1->id());
	auto rs41 = mOp4.rootSteps(mCI1->id());
	auto rs12 = mOp1.rootSteps(mCI2->id());
	auto rs22 = mOp2.rootSteps(mCI2->id());
	auto rs32 = mOp3.rootSteps(mCI2->id());
	auto rs42 = mOp4.rootSteps(mCI2->id());
	EXPECT_EQ(rs11.val(), 1u);
	EXPECT_EQ(rs21.val(), 1u);
	EXPECT_EQ(rs31.val(), 0u);
	EXPECT_EQ(rs41.val(), 0u);
	EXPECT_EQ(rs12.val(), 0u);
	EXPECT_EQ(rs22.val(), 0u);
	EXPECT_EQ(rs32.val(), 1u);
	EXPECT_EQ(rs42.val(), 1u);
    }

    TEST_F(OpCont_CR_Test, Assignment)
    {
	mOp1 = mOp2;
	mOp3 = mOp4;
	for(SizeT i = 0; i != mSize; ++i){
	    EXPECT_EQ(mOp1.data()[i], mOp2.data()[i]);
	    EXPECT_EQ(mOp3.data()[i], mOp4.data()[i]);
	}
	mOp1 = mOp4;
	mOp3 = mOp2;
	for(SizeT i = 0; i != mSize; ++i){
	    EXPECT_EQ(mOp1.data()[i], mOp4.data()[0]);
	    EXPECT_EQ(mOp3.data()[i], mOp2.data()[0]);
	}
    }

    TEST_F(OpCont_CR_Test, PlusAssign)
    {
	Vector<Double> z(mSize, 0);
	mOp1.init(mCI1, z);
	mOp3.init(mCI2, z);
	mOp1 += mOp2;
	mOp3 += mOp4;
	for(SizeT i = 0; i != mSize; ++i){
	    EXPECT_EQ(mOp1.data()[i], mOp2.data()[i]);
	    EXPECT_EQ(mOp3.data()[i], mOp4.data()[i]);
	}
	mOp1.init(mCI1, z);
	mOp3.init(mCI2, z);
	mOp1 = mOp4;
	mOp3 = mOp2;
	for(SizeT i = 0; i != mSize; ++i){
	    EXPECT_EQ(mOp1.data()[i], mOp4.data()[0]);
	    EXPECT_EQ(mOp3.data()[i], mOp2.data()[0]);
	}
    }

    TEST_F(OpCont_CR_CR_Test, Basics)
    {
	EXPECT_EQ(mOC1i1j.rootSteps(mCI1i->id()).val(), mCI1j->pmax().val());
	EXPECT_EQ(mOC1i1j.rootSteps(mCI1j->id()).val(), 1u);
	EXPECT_EQ(mOR1j1i.rootSteps(mCI1i->id()).val(), 1u);
	EXPECT_EQ(mOR1j1i.rootSteps(mCI1j->id()).val(), mCI1i->pmax().val());
    }

    TEST_F(OpCont_CR_CR_Test, Assignment)
    {
	mOC1i1j = mOR1j1i;
	for(SizeT i = 0; i != mCI1i->pmax().val(); ++i){
	    for(SizeT j = 0; j != mCI1j->pmax().val(); ++j){
		const SizeT jS = mCI1j->pmax().val();
		const SizeT iS = mCI1i->pmax().val();
		EXPECT_EQ(mOC1i1j.data()[i*jS+j], mOR1j1i.data()[j*iS+i]);
	    }
	}
    }

    TEST_F(OpCont_CR_CR_Test, Multiply)
    {
	mOC1i1j = mOR1j1i * mOR1i1j;
	for(SizeT i = 0; i != mCI1i->pmax().val(); ++i){
	    for(SizeT j = 0; j != mCI1j->pmax().val(); ++j){
		const SizeT jS = mCI1j->pmax().val();
		const SizeT iS = mCI1i->pmax().val();
		EXPECT_EQ(mOC1i1j.data()[i*jS+j], mOR1j1i.data()[j*iS+i] * mOR1i1j.data()[i*jS+j]);
	    }
	}
    }

    TEST_F(OpCont_CR_CR_Test2, Assignment)
    {
	EXPECT_EQ( mCC2j1i->stepSize( mCC1i2j->pack()[CSizeT<0>{}]->id() ).val(), 1u );
	EXPECT_EQ( mCC2j1i->stepSize( mCC1i2j->pack()[CSizeT<1>{}]->id() ).val(), 44u );
	EXPECT_EQ( mCC2j1i->stepSize( mCC1i2j->pack()[CSizeT<2>{}]->id() ).val(), 11u );

	mOC1i2j = mOR2j1i;
	
	for(SizeT i = 0; i != mCI1i->pmax().val(); ++i){
	    for(SizeT j = 0; j != mCI2j->pmax().val(); ++j){
		const SizeT jS = mCI2j->pmax().val();
		const SizeT iS = mCI1i->pmax().val();
		EXPECT_EQ(mOC1i2j.data()[i*jS+j], mOR2j1i.data()[j*iS+i]);
	    }
	}
	
    }

    TEST_F(OpCont_CR_CR_Test2, Multiply)
    {
	EXPECT_EQ( mCC2j1i->stepSize( mCC1i2j->pack()[CSizeT<0>{}]->id() ).val(), 1u );
	EXPECT_EQ( mCC2j1i->stepSize( mCC1i2j->pack()[CSizeT<1>{}]->id() ).val(), 44u );
	EXPECT_EQ( mCC2j1i->stepSize( mCC1i2j->pack()[CSizeT<2>{}]->id() ).val(), 11u );

	EXPECT_EQ( mCC1i2j->stepSize( mCC1i2j->pack()[CSizeT<0>{}]->id() ).val(), 12u );
	EXPECT_EQ( mCC1i2j->stepSize( mCC1i2j->pack()[CSizeT<1>{}]->id() ).val(), 4u );
	EXPECT_EQ( mCC1i2j->stepSize( mCC1i2j->pack()[CSizeT<2>{}]->id() ).val(), 1u );

	mOC1i2j = mOR2j1i * mOR1i2j;
	for(SizeT i = 0; i != mCI1i->pmax().val(); ++i){
	    for(SizeT j = 0; j != mCI2j->pmax().val(); ++j){
		const SizeT jS = mCI2j->pmax().val();
		const SizeT iS = mCI1i->pmax().val();
		EXPECT_EQ(mOC1i2j.data()[i*jS+j], mOR2j1i.data()[j*iS+i] * mOR1i2j.data()[i*jS+j]);
	    }
	}
    }
}
