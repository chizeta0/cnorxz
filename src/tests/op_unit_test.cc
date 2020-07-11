// -*- C++ -*-

#include "test_header.h"

namespace
{
    
    class OpTest_1Dim : public ::testing::Test
    {
    protected:

	typedef SingleRangeFactory<char,SpaceType::ANY> SRF;
	typedef SRF::oType SRange;

	OpTest_1Dim()
	{
	    swapFactory<SRF>(rfbptr, {'a', 'l', 'f', 'g'} );
	    srptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );
	}

	std::shared_ptr<RangeFactoryBase> rfbptr;
	std::shared_ptr<SRange> srptr;
	vector<double> v1 = { 2.917, 9.436, 0.373, 7.192 };
	vector<double> v2 = { 8.870, 4.790, 8.215, 5.063 };
    };
    
    class OpTest_MDim : public ::testing::Test
    {
    protected:

	typedef SingleRangeFactory<char,SpaceType::ANY> SRF;
	typedef SRF::oType SRange;

	typedef MultiRangeFactory<SRange,SRange> MRF;
	typedef MRF::oType MRange;

	typedef AnonymousRange ANO;
	typedef MultiArray<double,ANO> AMA;
	
	OpTest_MDim()
	{
	    swapFactory<SRF>(rfbptr, {'x', 'l', 'f', 'g'} );
	    sr1ptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

	    swapFactory<SRF>(rfbptr, {'1', '2', '3'} );
	    sr2ptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

	    swapFactory<SRF>(rfbptr, {'a', 'b'} );
	    sr3ptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

	    swapFactory<SRF>(rfbptr, {'A', 'B'} );
	    sr4ptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

	    swapMFactory<MRF>(rfbptr, sr2ptr, sr3ptr);
	    mr1ptr = std::dynamic_pointer_cast<MRange>( rfbptr->create() );

	    swapMFactory<MRF>(rfbptr, sr2ptr, sr4ptr);
	    mr2ptr = std::dynamic_pointer_cast<MRange>( rfbptr->create() );
	}

	std::shared_ptr<RangeFactoryBase> rfbptr;
	std::shared_ptr<SRange> sr1ptr;
	std::shared_ptr<SRange> sr2ptr;
	std::shared_ptr<SRange> sr3ptr;
	std::shared_ptr<SRange> sr4ptr;
	std::shared_ptr<MRange> mr1ptr;
	std::shared_ptr<MRange> mr2ptr;
	vector<double> v1 = { 2.917, 9.436, 0.373 };
	vector<double> v2 = { 8.870, 4.790 };
	vector<double> v3 = { 0.353, 4.005, 1.070, 2.310, 9.243, 2.911 };
	vector<double> v4 = { 1.470, 2.210 };
	vector<double> v5 = { 30.932, -33.693, -26.205, -15.504, 21.227, 17.829,
				   -14.364, -1.868, -25.703, 13.836, 23.563, 41.339 };
    };



    TEST_F(OpTest_1Dim, ExecOp)
    {
	MultiArray<double,SRange> ma1(srptr, v1);
	MultiArray<double,SRange> ma2(srptr, v2);
	MultiArray<double,SRange> res(srptr);

	auto i = MAT::getIndex( srptr );
	
	res(i) = ma1(i) + ma2(i);

	EXPECT_EQ( fabs( res.at('a') - (2.917+8.870) ) < 0.0001, true);
	EXPECT_EQ( fabs( res.at('l') - (9.436+4.790) ) < 0.0001, true );
	EXPECT_EQ( fabs( res.at('f') - (0.373+8.215) ) < 0.0001, true );
	EXPECT_EQ( fabs( res.at('g') - (7.192+5.063) ) < 0.0001, true );
    }

    
    TEST_F(OpTest_MDim, ExecOp1)
    {
	MultiArray<double,SRange,SRange> res(sr2ptr,sr4ptr);
	MultiArray<double,SRange,SRange> res2(sr2ptr,sr4ptr);
	const MultiArray<double,SRange> ma1(sr2ptr, v1);
	const MultiArray<double,SRange> ma2(sr4ptr, v2);

	auto i1 = MAT::getIndex( sr2ptr );
	auto i2 = MAT::getIndex( sr4ptr );

	res(i1,i2) = ma1(i1) * ma2(i2);

	EXPECT_EQ( xround( res.at(mkt('1','A')) ), xround(2.917 * 8.870) );
	EXPECT_EQ( xround( res.at(mkt('1','B')) ), xround(2.917 * 4.790) );

	EXPECT_EQ( xround( res.at(mkt('2','A')) ), xround(9.436 * 8.870) );
	EXPECT_EQ( xround( res.at(mkt('2','B')) ), xround(9.436 * 4.790) );
	
	EXPECT_EQ( xround( res.at(mkt('3','A')) ), xround(0.373 * 8.870) );
	EXPECT_EQ( xround( res.at(mkt('3','B')) ), xround(0.373 * 4.790) );

	res2(i1,i2) = (ma1(i1) + 3.14 ) * ma2(i2);

	EXPECT_EQ( xround( res2.at(mkt('1','A')) ), xround( (2.917 + 3.14) * 8.870) );
	EXPECT_EQ( xround( res2.at(mkt('1','B')) ), xround( (2.917 + 3.14) * 4.790) );

	EXPECT_EQ( xround( res2.at(mkt('2','A')) ), xround( (9.436 + 3.14) * 8.870) );
	EXPECT_EQ( xround( res2.at(mkt('2','B')) ), xround( (9.436 + 3.14) * 4.790) );
	
	EXPECT_EQ( xround( res2.at(mkt('3','A')) ), xround( (0.373 + 3.14) * 8.870) );
	EXPECT_EQ( xround( res2.at(mkt('3','B')) ), xround( (0.373 + 3.14) * 4.790) );
    }

    TEST_F(OpTest_MDim, ExecContract)
    {
	MultiArray<double,SRange> res(sr2ptr);
	const MultiArray<double,SRange> ma1(sr2ptr, v1);
	const MultiArray<double,SRange> ma2(sr4ptr, v2);

	auto i1 = MAT::getIndex( sr2ptr );
	auto i2 = MAT::getIndex( sr4ptr );

	res(i1) += (ma1(i1) * ma2(i2)).c(i2);

	EXPECT_EQ( xround( res.at('1') ), xround(2.917 * 8.870 + 2.917 * 4.790) );
	EXPECT_EQ( xround( res.at('2') ), xround(9.436 * 8.870 + 9.436 * 4.790) );
	EXPECT_EQ( xround( res.at('3') ), xround(0.373 * 8.870 + 0.373 * 4.790) );
    }
    
    TEST_F(OpTest_MDim, ExecOp2)
    {
	MultiArray<double,MRange,SRange> res(mr1ptr,sr4ptr);
	MultiArray<double,MRange> ma1(mr1ptr, v3);
	MultiArray<double,SRange> ma2(sr4ptr, v2);
	MultiArray<double,SRange> ma3(sr4ptr, v4);
	
	auto i1 = MAT::getIndex( mr1ptr );
	auto i2 = MAT::getIndex( sr4ptr );
	
	res(i1,i2) = ( ma1(i1) + ma2(i2) ).p(i1,i2) - ma3(i2);

	EXPECT_EQ( xround( res.at(mkt(mkt('1','a'),'A')) ), xround(0.353 + 8.870 - 1.470)  );
	EXPECT_EQ( xround( res.at(mkt(mkt('1','a'),'B')) ), xround(0.353 + 4.790 - 2.210)  );
	EXPECT_EQ( xround( res.at(mkt(mkt('1','b'),'A')) ), xround(4.005 + 8.870 - 1.470)  );
	EXPECT_EQ( xround( res.at(mkt(mkt('1','b'),'B')) ), xround(4.005 + 4.790 - 2.210) );

	EXPECT_EQ( xround( res.at(mkt(mkt('2','a'),'A')) ), xround(1.070 + 8.870 - 1.470)  );
	EXPECT_EQ( xround( res.at(mkt(mkt('2','a'),'B')) ), xround(1.070 + 4.790 - 2.210)  );
	EXPECT_EQ( xround( res.at(mkt(mkt('2','b'),'A')) ), xround(2.310 + 8.870 - 1.470)  );
	EXPECT_EQ( xround( res.at(mkt(mkt('2','b'),'B')) ), xround(2.310 + 4.790 - 2.210)  );

	EXPECT_EQ( xround( res.at(mkt(mkt('3','a'),'A')) ), xround(9.243 + 8.870 - 1.470)  );
	EXPECT_EQ( xround( res.at(mkt(mkt('3','a'),'B')) ), xround(9.243 + 4.790 - 2.210)  );
	EXPECT_EQ( xround( res.at(mkt(mkt('3','b'),'A')) ), xround(2.911 + 8.870 - 1.470)  );
	EXPECT_EQ( xround( res.at(mkt(mkt('3','b'),'B')) ), xround(2.911 + 4.790 - 2.210)  );
    }

    TEST_F(OpTest_MDim, ExecOpAnon)
    {
	MultiArray<double,MRange> ma1(mr1ptr, v3);
	MultiArray<double,AnonymousRange> maa
	    = *std::dynamic_pointer_cast<MultiArray<double,AnonymousRange>>( ma1.anonymous() );
	MultiArray<double,AnonymousRange> maa2( maa.template getRangePtr<0>() );

	auto ar = maa.template getRangePtr<0>();
	auto i1 = MAT::getIndex( ar );
	
	maa2(i1) = maa(i1) * maa(i1);
	EXPECT_EQ( xround( maa2.at(0) ), xround(  v3[0]*v3[0] ) );
	for((*i1) = 0; i1->pos() != i1->max(); ++(*i1) ){
	    EXPECT_EQ( xround( maa2.at(i1->meta()) ), xround( maa.at(i1->meta()) * maa.at(i1->meta()) ) );
	}
    }
    
    TEST_F(OpTest_MDim, ExecFOp)
    {
	MultiArray<double,MRange,SRange> res(mr1ptr,sr4ptr);
	MultiArray<double,MRange> ma1(mr1ptr, v3);
	MultiArray<double,SRange> ma2(sr4ptr, v2);
	MultiArray<double,SRange> ma3(sr4ptr, v4);
	
	auto i1 = MAT::getIndex( mr1ptr );
	auto i2 = MAT::getIndex( sr4ptr );
	
	res(i1,i2) = Monopole<double>::mk( ma1(i1) , ma2(i2) , ma3(i2) );

	EXPECT_EQ( xround( res.at(mkt(mkt('1','a'),'A')) ), xround(0.353 / ( 1 + 8.870 / 1.470) ) );
	EXPECT_EQ( xround( res.at(mkt(mkt('1','a'),'B')) ), xround(0.353 / ( 1 + 4.790 / 2.210) ) );
	EXPECT_EQ( xround( res.at(mkt(mkt('1','b'),'A')) ), xround(4.005 / ( 1 + 8.870 / 1.470) ) );
	EXPECT_EQ( xround( res.at(mkt(mkt('1','b'),'B')) ), xround(4.005 / ( 1 + 4.790 / 2.210) ) );

	EXPECT_EQ( xround( res.at(mkt(mkt('2','a'),'A')) ), xround(1.070 / ( 1 + 8.870 / 1.470) ) );
	EXPECT_EQ( xround( res.at(mkt(mkt('2','a'),'B')) ), xround(1.070 / ( 1 + 4.790 / 2.210) ) );
	EXPECT_EQ( xround( res.at(mkt(mkt('2','b'),'A')) ), xround(2.310 / ( 1 + 8.870 / 1.470) ) );
	EXPECT_EQ( xround( res.at(mkt(mkt('2','b'),'B')) ), xround(2.310 / ( 1 + 4.790 / 2.210) ) );

	EXPECT_EQ( xround( res.at(mkt(mkt('3','a'),'A')) ), xround(9.243 / ( 1 + 8.870 / 1.470) ) );
	EXPECT_EQ( xround( res.at(mkt(mkt('3','a'),'B')) ), xround(9.243 / ( 1 + 4.790 / 2.210) ) );
	EXPECT_EQ( xround( res.at(mkt(mkt('3','b'),'A')) ), xround(2.911 / ( 1 + 8.870 / 1.470) ) );
	EXPECT_EQ( xround( res.at(mkt(mkt('3','b'),'B')) ), xround(2.911 / ( 1 + 4.790 / 2.210) ) );
    }

    TEST_F(OpTest_MDim, ExecDOp)
    {
	//typedef MultiArray<double,AnonymousRange> AMA;
	typedef MultiArray<double,DynamicRange> DMA;
	
	MultiArray<double,MRange,SRange> ma1(mr1ptr,sr4ptr,v5);
	MultiArray<double,SRange> ma2(sr2ptr,v1);
	MultiArray<double,SRange,MRange> res(sr4ptr,mr1ptr);
	MultiArray<double,MRange,SRange> res2(mr1ptr,sr4ptr);

	DMA dma1 = *std::dynamic_pointer_cast<DMA>( dynamic( ma1 ) );
	DMA dma2 = *std::dynamic_pointer_cast<DMA>( dynamic( ma2 ) );
	DMA dres = *std::dynamic_pointer_cast<DMA>( dynamic( res ) );
	DMA dres2 = *std::dynamic_pointer_cast<DMA>( dynamic( res2 ) );
	
	auto si2 = MAT::getIndex( sr2ptr );
	auto si3 = MAT::getIndex( sr3ptr );
	auto si4 = MAT::getIndex( sr4ptr );
	auto mi = MAT::getIndex( mr1ptr );
	(*mi)(si2,si3);

	auto di1 = MAT::getIndex( MAT::rptr<0>( dma1 ) );
	auto di2 = MAT::getIndex( MAT::rptr<0>( dma2 ) );
	auto dir = MAT::getIndex( MAT::rptr<0>( dres ) );
	//auto dirx = MAT::getIndex( MAT::rptr<0>( dres ) );
	auto dir2 = MAT::getIndex( MAT::rptr<0>( dres2 ) );

	(*di1)(mi,si4);
	(*di2)(si2);
	(*dir)(si4,mi);
	(*dir2)(mi,si4);

	dres(dir) = dma1(di1) + dma2(di2);
	res = dres.format(sr4ptr,mr1ptr);
	
	EXPECT_EQ( xround( res.at( mkt('A',mkt('1','a')) ) ), xround( 30.932 + 2.917 ) );
	EXPECT_EQ( xround( res.at( mkt('A',mkt('1','b')) ) ), xround( -26.205 + 2.917 ) );
	EXPECT_EQ( xround( res.at( mkt('A',mkt('2','a')) ) ), xround( 21.227 + 9.436 ) );
	EXPECT_EQ( xround( res.at( mkt('A',mkt('2','b')) ) ), xround( -14.364 + 9.436 ) );
	EXPECT_EQ( xround( res.at( mkt('A',mkt('3','a')) ) ), xround( -25.703 + 0.373 ) );
	EXPECT_EQ( xround( res.at( mkt('A',mkt('3','b')) ) ), xround( 23.563 + 0.373 ) );

	EXPECT_EQ( xround( res.at( mkt('B',mkt('1','a')) ) ), xround( -33.693 + 2.917 ) );
	EXPECT_EQ( xround( res.at( mkt('B',mkt('1','b')) ) ), xround( -15.504 + 2.917 ) );
	EXPECT_EQ( xround( res.at( mkt('B',mkt('2','a')) ) ), xround( 17.829 + 9.436 ) );
	EXPECT_EQ( xround( res.at( mkt('B',mkt('2','b')) ) ), xround( -1.868 + 9.436 ) );
	EXPECT_EQ( xround( res.at( mkt('B',mkt('3','a')) ) ), xround( 13.836 + 0.373 ) );
	EXPECT_EQ( xround( res.at( mkt('B',mkt('3','b')) ) ), xround( 41.339 + 0.373 ) );

	//dres = *std::dynamic_pointer_cast<DMA>( dynamic( res ) );
	dres2(dir2) = dres(dir);
	res2 = dres2.format(mr1ptr,sr4ptr);
	
	EXPECT_EQ( xround( res2.at( mkt(mkt('1','a'),'A') ) ), xround( 30.932 + 2.917 ) );
	EXPECT_EQ( xround( res2.at( mkt(mkt('1','b'),'A') ) ), xround( -26.205 + 2.917 ) );
	EXPECT_EQ( xround( res2.at( mkt(mkt('2','a'),'A') ) ), xround( 21.227 + 9.436 ) );
	EXPECT_EQ( xround( res2.at( mkt(mkt('2','b'),'A') ) ), xround( -14.364 + 9.436 ) );
	EXPECT_EQ( xround( res2.at( mkt(mkt('3','a'),'A') ) ), xround( -25.703 + 0.373 ) );
	EXPECT_EQ( xround( res2.at( mkt(mkt('3','b'),'A') ) ), xround( 23.563 + 0.373 ) );

	EXPECT_EQ( xround( res2.at( mkt(mkt('1','a'),'B') ) ), xround( -33.693 + 2.917 ) );
	EXPECT_EQ( xround( res2.at( mkt(mkt('1','b'),'B') ) ), xround( -15.504 + 2.917 ) );
	EXPECT_EQ( xround( res2.at( mkt(mkt('2','a'),'B') ) ), xround( 17.829 + 9.436 ) );
	EXPECT_EQ( xround( res2.at( mkt(mkt('2','b'),'B') ) ), xround( -1.868 + 9.436 ) );
	EXPECT_EQ( xround( res2.at( mkt(mkt('3','a'),'B') ) ), xround( 13.836 + 0.373 ) );
	EXPECT_EQ( xround( res2.at( mkt(mkt('3','b'),'B') ) ), xround( 41.339 + 0.373 ) );

    }

    TEST_F(OpTest_MDim, ExecOp3)
    {
	MultiArray<double,MRange,SRange> res(mr1ptr,sr4ptr);
	MultiArray<double,MRange> ma1(mr1ptr, v3);
	MultiArray<double,SRange> ma2(sr2ptr, v1);
	MultiArray<double,SRange> ma3(sr4ptr, v4);
	MultiArray<double,SRange,SRange,SRange,SRange> ma4(sr1ptr,sr2ptr,sr3ptr,sr4ptr);
	MultiArray<double,SRange> ma5(sr1ptr, v3);
	MultiArray<double,SRange> ma6(sr3ptr, v2);
	
	auto si0 = MAT::getIndex( sr1ptr ); // 'x'
	auto si1 = MAT::getIndex( sr2ptr ); // '1'
	auto si2 = MAT::getIndex( sr3ptr ); // 'a'
	auto si3 = MAT::getIndex( sr4ptr ); // 'A'
	auto mi = MAT::getIndex( mr1ptr );
	mi->operator()(si1,si2);
	
	res(mi,si3) = ma1(mi) + ma2(si1) + ma3(si3);
	ma4(si0,si1,si2,si3) = ma5(si0)*ma2(si1)*ma6(si2)*ma3(si3);

	EXPECT_EQ( xround( res.at(mkt(mkt('1','a'),'A')) ), xround(0.353 + 2.917 + 1.470) );
	EXPECT_EQ( xround( res.at(mkt(mkt('1','a'),'B')) ), xround(0.353 + 2.917 + 2.210) );
	EXPECT_EQ( xround( res.at(mkt(mkt('1','b'),'A')) ), xround(4.005 + 2.917 + 1.470) );
	EXPECT_EQ( xround( res.at(mkt(mkt('1','b'),'B')) ), xround(4.005 + 2.917 + 2.210) );

	EXPECT_EQ( xround( res.at(mkt(mkt('2','a'),'A')) ), xround(1.070 + 9.436 + 1.470) );
	EXPECT_EQ( xround( res.at(mkt(mkt('2','a'),'B')) ), xround(1.070 + 9.436 + 2.210) );
	EXPECT_EQ( xround( res.at(mkt(mkt('2','b'),'A')) ), xround(2.310 + 9.436 + 1.470) );
	EXPECT_EQ( xround( res.at(mkt(mkt('2','b'),'B')) ), xround(2.310 + 9.436 + 2.210) );

	EXPECT_EQ( xround( res.at(mkt(mkt('3','a'),'A')) ), xround(9.243 + 0.373 + 1.470) );
	EXPECT_EQ( xround( res.at(mkt(mkt('3','a'),'B')) ), xround(9.243 + 0.373 + 2.210) );
	EXPECT_EQ( xround( res.at(mkt(mkt('3','b'),'A')) ), xround(2.911 + 0.373 + 1.470) );
	EXPECT_EQ( xround( res.at(mkt(mkt('3','b'),'B')) ), xround(2.911 + 0.373 + 2.210) );

	MultiArray<MultiArray<double,SRange,SRange>,SRange,SRange> ma7(sr2ptr,sr4ptr);
	ma7(si1,si3) = ma4(si0,si1,si2,si3).slc(si0,si2);

	si1->at('1');
	si0->at('x');
	(*si2) = 0;
	(*si3) = 0;
	Slice<double,SRange,SRange> sl(sr4ptr,sr3ptr);
	sl.define(si3,si2) = ma4(si0,si1,si2,si3);
	MultiArrayBase<double,SRange,SRange>& slb = sl;
	
	MultiArray<double,SRange,SRange> ma8(sr3ptr,sr4ptr);
	//ma8(si2,si3) = ma4(si0,si1,si2,si3);
	ma8(si2,si3) = slb(si3,si2);

	EXPECT_EQ( xround( ma8.at(mkt('a','A')) ), xround( sl.at(mkt('A','a')) ) );
	EXPECT_EQ( xround( ma8.at(mkt('b','A')) ), xround( sl.at(mkt('A','b')) ) );
	
	EXPECT_EQ( xround( ma7.at(mkt('1','A')).at(mkt('x','a')) ),
		   xround( ma4.at(mkt('x','1','a','A')) ) );
	EXPECT_EQ( xround( ma7.at(mkt('2','A')).at(mkt('x','a')) ),
		   xround( ma4.at(mkt('x','2','a','A')) ) );
	EXPECT_EQ( xround( ma7.at(mkt('3','A')).at(mkt('x','a')) ),
		   xround( ma4.at(mkt('x','3','a','A')) ) );

    	EXPECT_EQ( xround( ma7.at(mkt('1','A')).at(mkt('x','b')) ),
		   xround( ma4.at(mkt('x','1','b','A')) ) );
	EXPECT_EQ( xround( ma7.at(mkt('2','A')).at(mkt('x','b')) ),
		   xround( ma4.at(mkt('x','2','b','A')) ) );
	EXPECT_EQ( xround( ma7.at(mkt('3','A')).at(mkt('x','b')) ),
		   xround( ma4.at(mkt('x','3','b','A')) ) );

    	EXPECT_EQ( xround( ma7.at(mkt('1','A')).at(mkt('l','b')) ),
		   xround( ma4.at(mkt('l','1','b','A')) ) );
	EXPECT_EQ( xround( ma7.at(mkt('2','A')).at(mkt('l','b')) ),
		   xround( ma4.at(mkt('l','2','b','A')) ) );
	EXPECT_EQ( xround( ma7.at(mkt('3','A')).at(mkt('l','b')) ),
		   xround( ma4.at(mkt('l','3','b','A')) ) );

    	EXPECT_EQ( xround( ma7.at(mkt('1','B')).at(mkt('l','b')) ),
		   xround( ma4.at(mkt('l','1','b','B')) ) );
	EXPECT_EQ( xround( ma7.at(mkt('2','B')).at(mkt('l','b')) ),
		   xround( ma4.at(mkt('l','2','b','B')) ) );
	EXPECT_EQ( xround( ma7.at(mkt('3','B')).at(mkt('l','b')) ),
		   xround( ma4.at(mkt('l','3','b','B')) ) );
    }
    
} // anonymous namspace

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
