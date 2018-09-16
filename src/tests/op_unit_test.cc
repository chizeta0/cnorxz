// -*- C++ -*-

#include <cstdlib>
#include "gtest/gtest.h"
#include <iostream>

#include "multi_array_header.h"

#include <ctime>
#include <cmath>

namespace MAT = MultiArrayTools;

namespace {

    double xround(double arg)
    {
	if(std::isnan(arg)) { return 0.; }
	return roundf(arg * 100000.) / 100000.;
    }
    
    using namespace MAT;

    template <class Factory>
    void swapFactory(std::shared_ptr<RangeFactoryBase>& fptr)
    {
	auto nptr = std::make_shared<Factory>();
	fptr = nptr;
    }
    
    template <class Factory, typename T>
    void swapFactory(std::shared_ptr<RangeFactoryBase>& fptr, std::initializer_list<T> ilist)
    {
	std::vector<T> tmp = ilist;
	auto nptr = std::make_shared<Factory>( tmp );
	fptr = nptr;
    }

    template <class Factory, typename T>
    void swapFactory(std::shared_ptr<RangeFactoryBase>& fptr, std::vector<T>& ilist)
    {
	std::vector<T> tmp = ilist;
	auto nptr = std::make_shared<Factory>( tmp );
	fptr = nptr;
    }

    
    template <class Factory, class... Rs>
    void swapMFactory(std::shared_ptr<RangeFactoryBase>& fptr, const Rs&... rs)
    {
	auto nptr = std::make_shared<Factory>( rs... );
	fptr = nptr;
    }

    template <typename... Ts>
    auto mkt(Ts&&... ts) -> decltype(std::make_tuple(ts...))
    {
	return std::make_tuple(ts...);
    }

    template <typename... Ts>
    auto mkts(Ts&&... ts) -> decltype(std::make_tuple(ts...))
    {
	return std::make_tuple(static_cast<size_t>( ts )...);
    }

    template <typename T>
    struct Pow
    {
	static constexpr bool FISSTATIC = true;
	typedef T value_type;
	
	static inline T apply(T b, T e)
	{
	    return pow(b, e);
	}

	template <class... Ops>
	static auto mk(const Ops&... ops)
	    -> Operation<T,Pow<T>,Ops...>
	{
	    return Operation<T,Pow<T>,Ops...>(ops...);
	}

	static inline T apply(const std::tuple<double, double>& arg)
	{
	    return pow(std::get<0>(arg), std::get<1>(arg));
	}
    };

    
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
	std::vector<double> v1 = { 2.917, 9.436, 0.373, 7.192 };
	std::vector<double> v2 = { 8.870, 4.790, 8.215, 5.063 };
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
	std::vector<double> v1 = { 2.917, 9.436, 0.373 };
	std::vector<double> v2 = { 8.870, 4.790 };
	std::vector<double> v3 = { 0.353, 4.005, 1.070, 2.310, 9.243, 2.911 };
	std::vector<double> v4 = { 1.470, 2.210 };
    };

    class MapTest : public ::testing::Test
    {
    protected:

	typedef SingleRangeFactory<size_t,SpaceType::ANY> SRF;
	typedef SRF::oType SRange;

	typedef FunctionalMultiArray<size_t,plus<size_t>,SRange,SRange> MapF;
	
	typedef MapRangeFactory<MapF,SRange,SRange> MpRF;
	typedef MpRF::oType MpRange;

	typedef MpRange::ORType TRange;
	
	MapTest()
	{
	    SRF srf1( { 1,  3,  7,  10 } );
	    SRF srf2( { 2,  6,  8 } );

	    sr1ptr = std::dynamic_pointer_cast<SRange>( srf1.create() );
	    sr2ptr = std::dynamic_pointer_cast<SRange>( srf2.create() );

	    MapF map(sr1ptr,sr2ptr);
	    MpRF mprf1( map, sr1ptr, sr2ptr );

	    mpr1ptr = std::dynamic_pointer_cast<MpRange>( mprf1.create() );
	}

	std::shared_ptr<SRange> sr1ptr;
	std::shared_ptr<SRange> sr2ptr;
	std::shared_ptr<MpRange> mpr1ptr;
	std::vector<double> v1 = { -31.71, -77.16, -18.81,
				   -67.06, 72.31, -54.48,
				   -50.91, -11.62, -59.57,
				   -42.53, 80.41, 6.35 };
    };
    
    class OpTest_Performance : public ::testing::Test
    {
    protected:

	typedef SingleRangeFactory<size_t,SpaceType::ANY> SRF;
	typedef SRF::oType SRange;

	typedef MultiRangeFactory<SRange,SRange> MRF;
	typedef MRF::oType MRange;

	OpTest_Performance()
	{
	
	    std::vector<size_t> initvec1(vs1);
	    cv1.resize(vs1);
	    for(size_t i = 0; i != vs1; ++i){
		initvec1[i] = i;
		cv1[i] = sqrt( static_cast<double>(i)*0.53 );
	    }

	    std::vector<size_t> initvec2(vs2);
	    cv2.resize(vs2*vs1);
	    for(size_t i = 0; i != vs2; ++i){
		initvec2[i] = i;
		for(size_t j = 0; j != vs1; ++j){
		    cv2[i*vs1 + j] = static_cast<double>(i) * sin(static_cast<double>(j)*0.4);
		}
	    }

	    swapFactory<SRF>(rfbptr, initvec1);
	    sr1ptr = std::dynamic_pointer_cast<SRange>(rfbptr->create());

	    swapFactory<SRF>(rfbptr, initvec2);
	    sr2ptr = std::dynamic_pointer_cast<SRange>(rfbptr->create());

	    swapMFactory<MRF>(rfbptr, sr2ptr, sr1ptr);
	    mrptr = std::dynamic_pointer_cast<MRange>(rfbptr->create());
	}

	//const size_t vs1 = 10000;
	//const size_t vs2 = 1000;
	const size_t vs1 = 4000;
	const size_t vs2 = 2500;

	std::shared_ptr<RangeFactoryBase> rfbptr;
	std::shared_ptr<SRange> sr1ptr;
	std::shared_ptr<SRange> sr2ptr;
	std::shared_ptr<MRange> mrptr;

	std::vector<double> cv1;
	std::vector<double> cv2;
    };

    class OpTest_Spin : public ::testing::Test
    {
    protected:

	typedef SpinRF SRF;
	typedef SpinRange SR;
	typedef MultiRangeFactory<SR,SR,SR,SR,SR,SR,SR,SR> SR8F;
	typedef SR8F::oType SR8;

	static const size_t s = 65536;
	
	OpTest_Spin()
	{
	    data.resize(s);
	    for(size_t i = 0; i != s; ++i){
		double arg = static_cast<double>( i - s ) - 0.1; 
		data[i] = sin(arg)/arg;
	    }
	    SRF f;
	    sr = std::dynamic_pointer_cast<SR>(f.create());
	}

	std::vector<double> data;
	std::shared_ptr<SR> sr;
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

	std::vector<double> mv1;
	std::vector<double> mv2;

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

	auto mr = mkMapR(std::make_shared<plus<size_t>>(),sr1ptr,sr2ptr);
	MultiArray<double,MpRange> res(mr);
	auto jj = getIndex( mr );
	(*jj)(ii1,ii2);
	///auto jj = mkMapI( std::make_shared<plus<size_t> >(), ii1, ii1 );
		
	res(jj) = ma1(ii1,ii2);

	MultiArray<double,TRange> form = res.format(mpr1ptr->outRange());

	EXPECT_EQ( jj->range()->outRange()->size(), 10 );
 	EXPECT_EQ( jj->range()->mapMultiplicity().at(9), 3 );
	EXPECT_EQ( jj->range()->mapMultiplicity().at(3), 1 );

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
	
	res(i1) = fma(i1,i2).c(i2);

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

    
    TEST_F(OpTest_Spin, Contract)
    {
	MultiArray<double,SR,SR,SR,SR,SR,SR,SR,SR> ma(sr, sr, sr, sr, sr, sr, sr, sr, data);
	MultiArray<double,SR,SR> res1( sr, sr );
	MultiArray<double,SR,SR> res2( sr, sr );
	
	auto alpha = MAT::getIndex<SR>();
	auto beta = MAT::getIndex<SR>();
	auto gamma = MAT::getIndex<SR>();
	auto delta = MAT::getIndex<SR>();
	auto deltap = MAT::getIndex<SR>();
	
	auto mix = MAT::mkMIndex( alpha, beta, gamma );

	std::clock_t begin = std::clock();
	res1(delta, deltap) = ma(delta, alpha, alpha, beta, beta, gamma, gamma, deltap).c(mix);
	std::clock_t end = std::clock();
	std::cout << "MultiArray time: " << static_cast<double>( end - begin ) / CLOCKS_PER_SEC
		  << std::endl;

	res2(delta, deltap) = ma(delta, alpha, alpha, beta, beta, gamma, gamma, deltap).c(alpha).c(beta).c(gamma);
	
	std::vector<double> vres(4*4);

	std::clock_t begin2 = std::clock();
	for(size_t d = 0; d != 4; ++d){
	    for(size_t p = 0; p != 4; ++p){
		const size_t tidx = d*4 + p;
		vres[tidx] = 0.;
		for(size_t a = 0; a != 4; ++a){
		    for(size_t b = 0; b != 4; ++b){
			for(size_t c = 0; c != 4; ++c){
			    const size_t sidx = d*4*4*4*4*4*4*4 + a*5*4*4*4*4 + b*5*4*4*4 + c*5*4 + p;
			    vres[tidx] += data[sidx];
			}
		    }
		}
	    }
	}
	std::clock_t end2 = std::clock();

	EXPECT_EQ( xround(res1.at(mkts(0,0))), xround(vres[0]) );
	EXPECT_EQ( xround(res1.at(mkts(0,1))), xround(vres[1]) );
	EXPECT_EQ( xround(res1.at(mkts(0,2))), xround(vres[2]) );
	EXPECT_EQ( xround(res1.at(mkts(0,3))), xround(vres[3]) );
	
	EXPECT_EQ( xround(res1.at(mkts(1,0))), xround(vres[4]) );
	EXPECT_EQ( xround(res1.at(mkts(1,1))), xround(vres[5]) );
	EXPECT_EQ( xround(res1.at(mkts(1,2))), xround(vres[6]) );
	EXPECT_EQ( xround(res1.at(mkts(1,3))), xround(vres[7]) );

	EXPECT_EQ( xround(res1.at(mkts(2,0))), xround(vres[8]) );
	EXPECT_EQ( xround(res1.at(mkts(2,1))), xround(vres[9]) );
	EXPECT_EQ( xround(res1.at(mkts(2,2))), xround(vres[10]) );
	EXPECT_EQ( xround(res1.at(mkts(2,3))), xround(vres[11]) );

	EXPECT_EQ( xround(res1.at(mkts(3,0))), xround(vres[12]) );
	EXPECT_EQ( xround(res1.at(mkts(3,1))), xround(vres[13]) );
	EXPECT_EQ( xround(res1.at(mkts(3,2))), xround(vres[14]) );
	EXPECT_EQ( xround(res1.at(mkts(3,3))), xround(vres[15]) );

	EXPECT_EQ( xround(res2.at(mkts(0,0))), xround(vres[0]) );
	EXPECT_EQ( xround(res2.at(mkts(0,1))), xround(vres[1]) );
	EXPECT_EQ( xround(res2.at(mkts(0,2))), xround(vres[2]) );
	EXPECT_EQ( xround(res2.at(mkts(0,3))), xround(vres[3]) );
	
	EXPECT_EQ( xround(res2.at(mkts(1,0))), xround(vres[4]) );
	EXPECT_EQ( xround(res2.at(mkts(1,1))), xround(vres[5]) );
	EXPECT_EQ( xround(res2.at(mkts(1,2))), xround(vres[6]) );
	EXPECT_EQ( xround(res2.at(mkts(1,3))), xround(vres[7]) );

	EXPECT_EQ( xround(res2.at(mkts(2,0))), xround(vres[8]) );
	EXPECT_EQ( xround(res2.at(mkts(2,1))), xround(vres[9]) );
	EXPECT_EQ( xround(res2.at(mkts(2,2))), xround(vres[10]) );
	EXPECT_EQ( xround(res2.at(mkts(2,3))), xround(vres[11]) );

	EXPECT_EQ( xround(res2.at(mkts(3,0))), xround(vres[12]) );
	EXPECT_EQ( xround(res2.at(mkts(3,1))), xround(vres[13]) );
	EXPECT_EQ( xround(res2.at(mkts(3,2))), xround(vres[14]) );
	EXPECT_EQ( xround(res2.at(mkts(3,3))), xround(vres[15]) );

	std::cout << "std::vector - for loop time: " << static_cast<double>( end2 - begin2 ) / CLOCKS_PER_SEC
		  << std::endl;
	std::cout << "ratio: " << static_cast<double>( end - begin ) / static_cast<double>( end2 - begin2 ) << std::endl;
    }    
    
    TEST_F(OpTest_Performance, PCheck)
    {
	MultiArray<double,MRange> ma2(mrptr, cv2);
	MultiArray<double,SRange> ma1(sr1ptr, cv1);
	MultiArray<double,MRange> res(mrptr);

	auto si1 = MAT::getIndex(sr1ptr);
	auto si2 = MAT::getIndex(sr2ptr);
	auto mi = MAT::getIndex(mrptr);
	(*mi)(si2, si1);

	std::clock_t begin = std::clock();
	
	res(mi) = ma2(mi) * ma1(si1);

	std::clock_t end = std::clock();
	std::cout << "MultiArray time: " << static_cast<double>( end - begin ) / CLOCKS_PER_SEC
		  << std::endl;

	std::vector<double> res2(vs1*vs2);
	std::clock_t begin2 = std::clock();
	
	for(size_t i = 0; i != vs2; ++i){
	    for(size_t j = 0; j != vs1; ++j){
		res2[i*vs1 + j] = cv1[j] * cv2[i*vs1 + j];
	    }
	}

	std::clock_t end2 = std::clock();
	std::cout << "std::vector - for loop time: " << static_cast<double>( end2 - begin2 ) / CLOCKS_PER_SEC
		  << std::endl;

	std::cout << "ratio: " << static_cast<double>( end - begin ) / static_cast<double>( end2 - begin2 ) << std::endl;

	EXPECT_EQ( xround( res.at(mkt(7,9)) ), xround(res2[7*vs1 + 9]) );
	//EXPECT_EQ( xround( res.at(mkt(700,900)) ), xround(res2[700*vs1 + 900]) );
	
    }

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

	res(i1) = (ma1(i1) * ma2(i2)).c(i2);

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
	
	res(i1,i2) = ( ma1(i1) + ma2(i2) ) - ma3(i2);

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

    template <typename T>
    struct Monopole
    {
	static constexpr bool FISSTATIC = true;

	static inline T apply(T f0, T x, T n)
	{
	    return f0 / ( 1 + x / n );
	}

	template <class... Ops>
	static auto mk(const Ops&... ops)
	    -> Operation<T,Monopole<T>,Ops...>
	{
	    return Operation<T,Monopole<T>,Ops...>(ops...);
	}
    };
    
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

    TEST_F(OpTest_MDim, ExecOp3)
    {
	MultiArray<double,MRange,SRange> res(mr1ptr,sr4ptr);
	MultiArray<double,MRange> ma1(mr1ptr, v3);
	MultiArray<double,SRange> ma2(sr2ptr, v1);
	MultiArray<double,SRange> ma3(sr4ptr, v4);
	MultiArray<double,SRange,SRange,SRange,SRange> ma4(sr1ptr,sr2ptr,sr3ptr,sr4ptr);
	MultiArray<double,SRange> ma5(sr1ptr, v3);
	MultiArray<double,SRange> ma6(sr3ptr, v2);
	
	auto si0 = MAT::getIndex( sr1ptr );
	auto si1 = MAT::getIndex( sr2ptr );
	auto si2 = MAT::getIndex( sr3ptr );
	auto si3 = MAT::getIndex( sr4ptr );
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
	si3->at('A');
	Slice<double,SRange,SRange> sl(sr1ptr,sr3ptr);
	sl.define(si2,si3) = ma4(si0,si1,si2,si3);

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
    /*
    TEST_F(OpTest_MDim, ExecAnonOp1)
    {
	MultiArray<double,MRange,SRange> res(mr1ptr,sr4ptr);
	MultiArray<double,MRange> ma1(mr1ptr, v3);
	MultiArray<double,SRange> ma2(sr2ptr, v1);
	MultiArray<double,SRange> ma3(sr4ptr, v4);

	auto si1 = MAT::getIndex( sr2ptr );
	auto si2 = MAT::getIndex( sr3ptr );
	auto si3 = MAT::getIndex( sr4ptr );
	auto mi = MAT::getIndex( mr1ptr );
	mi->operator()(si1,si2);
	
	res(mi,si3) = ma1(mi) + ma2(si1) + ma3(si3);

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
    }
    */
    
} // anonymous namspace

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
