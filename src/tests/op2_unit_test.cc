
#include "test_header.h"

namespace
{
    class OpTest_Sub : public ::testing::Test
    {
    protected:

	typedef SingleRangeFactory<char,SpaceType::ANY> SRF;
	typedef SRF::oType SRange;

        OpTest_Sub()
	{
	    swapFactory<SRF>(rfbptr, {'x', 'l'} );
	    sr1ptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

	    swapFactory<SRF>(rfbptr, {'1', '2', '3'} );
	    sr2ptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

	    swapFactory<SRF>(rfbptr, {'a', 'b'} );
	    sr3ptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

        }

	std::shared_ptr<RangeFactoryBase> rfbptr;
        std::shared_ptr<SRange> sr1ptr;
	std::shared_ptr<SRange> sr2ptr;
	std::shared_ptr<SRange> sr3ptr;

        vector<double> v1 = { 2.917, 9.436, 0.373, 0.353, 4.005, 1.070,
                                   -14.364, -1.868, -25.703, 13.836, 23.563, 41.339 };

        vector<double> v2 = { 0.353, 4.005, 1.070, 2.310, 9.243, 2.911 };
    };

    
    class MapTest : public ::testing::Test
    {
    protected:

	typedef SingleRangeFactory<size_t,SpaceType::ANY> SRF;
	typedef SRF::oType SRange;
        typedef typename SRange::IndexType SIndex;

        std::shared_ptr<SRange> sr1ptr;
	std::shared_ptr<SRange> sr2ptr;
        std::shared_ptr<SIndex> si1ptr;
        std::shared_ptr<SIndex> si2ptr;
        
	vector<double> v1 = { -31.71, -77.16, -18.81,
                              -67.06, 72.31, -54.48,
                              -50.91, -11.62, -59.57,
                              -42.53, 80.41, 6.35 };

        typedef std::remove_reference<decltype(*mkMapR( mkMapOp(std::make_shared<plus<size_t>>(),getIndex(sr1ptr),getIndex(sr2ptr)) , sr1ptr, sr2ptr ))>::type MpRange;
        std::shared_ptr<MpRange> mpr1ptr;
        typedef MpRange::ORType TRange;
        
	MapTest()
	{
	    SRF srf1( { 1,  3,  7,  10 } );
	    SRF srf2( { 2,  6,  8 } );

	    sr1ptr = std::dynamic_pointer_cast<SRange>( srf1.create() );
	    sr2ptr = std::dynamic_pointer_cast<SRange>( srf2.create() );
            si1ptr = getIndex(sr1ptr);
            si2ptr = getIndex(sr2ptr);
            
            mpr1ptr = mkMapR( mkMapOp(std::make_shared<plus<size_t>>(),si1ptr,si2ptr) , sr1ptr, sr2ptr );
	}

    };
    

    class MetaOp_Test : public ::testing::Test
    {
    protected:

	typedef SingleRangeFactory<double,SpaceType::ANY> SRF;
	typedef SRF::oType SR;

	const size_t s1 = 10;
	const size_t s2 = 17;
	
	MetaOp_Test()
	{
	    mv1 = { 2.476, 9.665, 1.289, 2.89, 77.04, -11.09, 100.4, 2.0, -26.5, -0.001 };
	    mv2 = { 44.56, 23.097, -117.3, -0.0765, 3.445, 0.02389, -4.0112, 10.567, 8.99, -177.2, 475.3,
		    11.111, 13.108, -35.6, 64.32, 2.44, -12.};

	    assert(mv1.size() == s1); // just to prevent typos...
	    assert(mv2.size() == s2);
	    
	    swapFactory<SRF>(rfbptr, mv1);
	    sr1ptr = std::dynamic_pointer_cast<SR>(rfbptr->create());

	    swapFactory<SRF>(rfbptr, mv2);
	    sr2ptr = std::dynamic_pointer_cast<SR>(rfbptr->create());
	}

	std::shared_ptr<RangeFactoryBase> rfbptr;
	std::shared_ptr<SR> sr1ptr;
	std::shared_ptr<SR> sr2ptr;

	vector<double> mv1;
	vector<double> mv2;

	double vcontract(size_t i)
	{
	    double res = 0.;
	    for(auto& x: mv2){
		res += pow(mv1[i],x);
	    }
	    return res;
	}
    };

    TEST_F(MapTest, Exec1)
    {
	MultiArray<double,SRange,SRange> ma1(sr1ptr,sr2ptr,v1);


	auto ii1 = getIndex( rptr<0>( ma1 ) );
	auto ii2 = getIndex( rptr<1>( ma1 ) );
        
        auto mr = mkMapR( mkMapOp(std::make_shared<plus<size_t>>(),ii1,ii2) , sr1ptr, sr2ptr );
	//auto mr = mkMapR(std::make_shared<plus<size_t>>(),sr1ptr,sr2ptr);
	MultiArray<double,MpRange> res(mr);
	MultiArray<double,MpRange> res2(mr);
	auto jj = getIndex( mr );
	(*jj)(ii1,ii2);
	///auto jj = mkMapI( std::make_shared<plus<size_t> >(), ii1, ii1 );
		
	res(jj) += ma1(ii1,ii2);
	auto mult = mr->explMapMultiplicity();
        res2(jj) += ma1(ii1,ii2) / staticcast<double>( mult(jj) );

	MultiArray<double,TRange> form = res.format(mpr1ptr->outRange());
	MultiArray<double,TRange> form2 = res2.format(mpr1ptr->outRange());
	
	EXPECT_EQ( jj->range()->outRange()->size(), static_cast<size_t>( 10 ) );
 	EXPECT_EQ( jj->range()->mapMultiplicity().at(9), static_cast<size_t>( 3 ) );
	EXPECT_EQ( jj->range()->mapMultiplicity().at(3), static_cast<size_t>( 1 ) );
        
	EXPECT_EQ( form.at(3), -31.71 );
	EXPECT_EQ( form.at(7), -77.16 );
	EXPECT_EQ( form.at(9), -18.81 + 72.31 -50.91 );
	EXPECT_EQ( form.at(5), -67.06 );
	EXPECT_EQ( form.at(11), -54.48 );
	EXPECT_EQ( form.at(13), -11.62 );
	EXPECT_EQ( form.at(15), -59.57 );
	EXPECT_EQ( form.at(12), -42.53 );
	EXPECT_EQ( form.at(16), 80.41 );
	EXPECT_EQ( form.at(18), 6.35 );
        
	EXPECT_EQ( form2.at(3), -31.71 );
	EXPECT_EQ( form2.at(7), -77.16 );
	EXPECT_EQ( xround( form2.at(9) ), xround( (-18.81 + 72.31 -50.91) / 3. ) );
	EXPECT_EQ( form2.at(5), -67.06 );
	EXPECT_EQ( form2.at(11), -54.48 );
	EXPECT_EQ( form2.at(13), -11.62 );
	EXPECT_EQ( form2.at(15), -59.57 );
	EXPECT_EQ( form2.at(12), -42.53 );
	EXPECT_EQ( form2.at(16), 80.41 );
	EXPECT_EQ( form2.at(18), 6.35 );
    }
    
    TEST_F(MetaOp_Test, SimpleCall)
    {
	FunctionalMultiArray<double,Pow<double>,SR,SR> fma(sr1ptr, sr2ptr);

	auto i = fma.begin();
	
	EXPECT_EQ( xround( fma[ i.at( mkt(9.665, -0.0765) ) ] ), xround( pow(9.665, -0.0765) ) );
    }

    TEST_F(MetaOp_Test, Operation)
    {
	FunctionalMultiArray<double,Pow<double>,SR,SR> fma(sr1ptr, sr2ptr);
	MultiArray<double,SR> res( sr1ptr );

	auto i1 = MAT::getIndex(sr1ptr);
	auto i2 = MAT::getIndex(sr2ptr);
	
	res(i1) += fma(i1,i2).c(i2);

	auto i = res.begin();
	
	EXPECT_EQ( xround( res[ i.at( 2.476 ) ] ), xround( vcontract(0) ) );
	EXPECT_EQ( xround( res[ i.at( 9.665 ) ] ), xround( vcontract(1) ) );
	EXPECT_EQ( xround( res[ i.at( 1.289 ) ] ), xround( vcontract(2) ) );
	EXPECT_EQ( xround( res[ i.at( 2.89 ) ] ), xround( vcontract(3) ) );
	EXPECT_EQ( xround( res[ i.at( 77.04 ) ] ), xround( vcontract(4) ) );
	
	EXPECT_EQ( xround( res[ i.at( -11.09 ) ] ), xround( vcontract(5) ) );
	EXPECT_EQ( xround( res[ i.at( 100.4 ) ] ), xround( vcontract(6) ) );
	EXPECT_EQ( xround( res[ i.at( 2.0 ) ] ), xround( vcontract(7) ) );
	EXPECT_EQ( xround( res[ i.at( -26.5 ) ] ), xround( vcontract(8) ) );
	EXPECT_EQ( xround( res[ i.at( -0.001 ) ] ), xround( vcontract(9) ) );
    }

    TEST_F(OpTest_Sub, Exec)
    {
        MultiArray<double,SRange,SRange,SRange> ma1(sr1ptr, sr2ptr, sr3ptr, v1);
        MultiArray<double,SRange,SRange> ma2(sr3ptr, sr2ptr, v2);

        SubRangeFactory<SRange> subf(sr2ptr, vector<size_t>({0,2}));
        auto subptr = MAT::createExplicit(subf);
        
        MultiArray<double,SRange,SRange> res(sr3ptr,sr1ptr,0.);
	MultiArray<double,SRange,SubRange<SRange>,SRange> res2(sr3ptr,subptr,sr1ptr,0.);

        auto i1 = MAT::getIndex( sr1ptr );
        auto i2 = MAT::getIndex( sr2ptr );
        auto i3 = MAT::getIndex( sr3ptr );
        auto si = MAT::getIndex( subptr );
        (*si)(i2);

        res(i3,i1) += (ma2(i3,i2) - ma1(i1,i2,i3)).c(si);
	res2(i3,si,i1) = ma2(i3,i2) - ma1(i1,i2,i3);

        EXPECT_EQ( res2.size(), static_cast<size_t>(8) );
        EXPECT_EQ( res2.vdata().size(), static_cast<size_t>(8) );
        EXPECT_EQ( MAT::rptr<1>( res2 )->size(), static_cast<size_t>(2) );

        EXPECT_EQ( MAT::rptr<1>( res2 )->isMeta('1'), true );
        EXPECT_EQ( MAT::rptr<1>( res2 )->isMeta('3'), true );
        
        EXPECT_EQ( xround( res.at(mkt('a','x')) ), xround((ma2.at(mkt('a','1')) - ma1.at(mkt('x','1','a'))) + (ma2.at(mkt('a','3')) - ma1.at(mkt('x','3','a')) ) ));
	EXPECT_EQ( xround( res.at(mkt('a','l')) ), xround((ma2.at(mkt('a','1')) - ma1.at(mkt('l','1','a'))) + (ma2.at(mkt('a','3')) - ma1.at(mkt('l','3','a')) ) ));

        EXPECT_EQ( xround( res.at(mkt('b','x')) ), xround((ma2.at(mkt('b','1')) - ma1.at(mkt('x','1','b'))) + (ma2.at(mkt('b','3')) - ma1.at(mkt('x','3','b')) ) ) );
        EXPECT_EQ( xround( res.at(mkt('b','l')) ), xround((ma2.at(mkt('b','1')) - ma1.at(mkt('l','1','b'))) + (ma2.at(mkt('b','3')) - ma1.at(mkt('l','3','b')) ) ) );

	EXPECT_EQ( xround( res2.at(mkt('a','1','x')) ), xround(ma2.at(mkt('a','1')) - ma1.at(mkt('x','1','a'))) );
        EXPECT_EQ( xround( res2.at(mkt('a','1','l')) ), xround(ma2.at(mkt('a','1')) - ma1.at(mkt('l','1','a'))) );

        EXPECT_EQ( xround( res2.at(mkt('b','1','x')) ), xround(ma2.at(mkt('b','1')) - ma1.at(mkt('x','1','b'))) );
        EXPECT_EQ( xround( res2.at(mkt('b','1','l')) ), xround(ma2.at(mkt('b','1')) - ma1.at(mkt('l','1','b'))) );

        EXPECT_EQ( xround( res2.at(mkt('a','3','x')) ), xround(ma2.at(mkt('a','3')) - ma1.at(mkt('x','3','a'))) );
        EXPECT_EQ( xround( res2.at(mkt('a','3','l')) ), xround(ma2.at(mkt('a','3')) - ma1.at(mkt('l','3','a'))) );

        EXPECT_EQ( xround( res2.at(mkt('b','3','x')) ), xround(ma2.at(mkt('b','3')) - ma1.at(mkt('x','3','b'))) );
        EXPECT_EQ( xround( res2.at(mkt('b','3','l')) ), xround(ma2.at(mkt('b','3')) - ma1.at(mkt('l','3','b'))) );

    }

}
