// -*- C++ -*-

#include <cstdlib>
#include "gtest/gtest.h"
#include <iostream>

#include "multi_array_header.h"

namespace MAT = MultiArrayTools;

namespace {

    using namespace MAT;

    template <class Factory, typename T>
    void swapFactory(std::shared_ptr<RangeFactoryBase>& fptr, std::initializer_list<T> ilist)
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
   
    
    class MATest_1Dim : public ::testing::Test
    {
    protected:

	typedef SingleRangeFactory<char,SpaceType::ANY> SRF;
	typedef SRF::oType SRange;

	MATest_1Dim()
	{
	    swapFactory<SRF>(rfbptr, {'x', 'y', 'l', 'f', 'g'} );
	    srptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

	}

	std::shared_ptr<RangeFactoryBase> rfbptr;
	std::shared_ptr<SRange> srptr;
	std::vector<double> vv = { 3.141, 2.718, 1.618, 0.693, 0.577 };
    };

    
    class MATest_MDim : public ::testing::Test
    {
    protected:

	typedef SingleRangeFactory<char,SpaceType::ANY> SRF;
	typedef SRF::oType SRange;

	typedef MultiRangeFactory<SRange,SRange> MRF;
	typedef MRF::oType MRange;
	
	MATest_MDim()
	{
	    swapFactory<SRF>(rfbptr, {'x', 'y'} );
	    sr1ptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

	    swapFactory<SRF>(rfbptr, {'a', 'l', 'f', 'g'} );
	    sr2ptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

	    swapFactory<SRF>(rfbptr, {'1', '2', '3'} );
	    sr3ptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

	    swapFactory<SRF>(rfbptr, { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X' } );
	    sr4ptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

	    swapMFactory<MRF>(rfbptr, sr1ptr, sr2ptr);
	    mrptr = std::dynamic_pointer_cast<MRange>( rfbptr->create() );
	    
	}
	
	std::shared_ptr<RangeFactoryBase> rfbptr;
	std::shared_ptr<SRange> sr1ptr;
	std::shared_ptr<SRange> sr2ptr;
	std::shared_ptr<SRange> sr3ptr;
	std::shared_ptr<SRange> sr4ptr;
	std::shared_ptr<MRange> mrptr;
	std::vector<double> vv = { 2.917, 9.436, 0.373, 7.192, 7.315, 1.536, 4.892, 0.280,
				   8.870, 4.790, 8.215, 5.063, 1.530, 3.084, 1.609, 4.847,
				   8.175, 0.112, 6.712, 6.408, 1.959, 0.331, 4.209, 2.951 };
    };
    
    TEST_F(MATest_1Dim, SimpleCall)
    {
	MultiArray<double,MATest_1Dim::SRange> ma(srptr, vv);
	EXPECT_EQ( ma.size(), 5u);
	EXPECT_EQ( ma.isConst(), false);
	EXPECT_EQ( ma.isSlice(), false);	
	
	auto i = ma.beginIndex();
	EXPECT_EQ( ma[ i.at('x') ], 3.141);
	EXPECT_EQ( ma[ i.at('y') ], 2.718);
	EXPECT_EQ( ma[ i.at('l') ], 1.618);
	EXPECT_EQ( ma[ i.at('f') ], 0.693);
	EXPECT_EQ( ma[ i.at('g') ], 0.577);

    }
    
    TEST_F(MATest_1Dim, ForLoop)
    {
	std::vector<double> v2 = { 0.693 , 2.718, 3.141, 1.618, 9.98 };
	MultiArray<double,MATest_1Dim::SRange> ma(srptr, std::move( v2 ) );
	size_t cnt = 0;
	for(auto el: ma){

	    if(cnt == 0){
		EXPECT_EQ(el, 0.693);
	    } else if(cnt == 1u){
		EXPECT_EQ(el, 2.718);
	    } else if(cnt == 2u){
		EXPECT_EQ(el, 3.141);
	    } else if(cnt == 3u){
		EXPECT_EQ(el, 1.618);
	    } else if(cnt == 4u){
		EXPECT_EQ(el, 9.98);
	    } else {
		EXPECT_EQ(cnt, 0u);
	    }
	    ++cnt;
	}
	EXPECT_EQ(cnt, ma.size());
    }
    
    TEST_F(MATest_1Dim, ReFormat)
    {
	swapFactory<SRF>( rfbptr, { 'a', 'c', 'e', 'g', 'i' } );
	std::shared_ptr<SRange> sr2 = std::dynamic_pointer_cast<SRange>( rfbptr->create() );
	
	MultiArray<double,MATest_1Dim::SRange> ma(srptr, vv);
	auto i = ma.beginIndex();
	EXPECT_EQ( ma[ i.at('x') ], 3.141);

	auto ma2 = ma.format( sr2 );
	auto j = ma2.beginIndex();
	
	EXPECT_EQ( ma[ j.at('a') ], 3.141);
	EXPECT_EQ( ma[ j.at('c') ], 2.718);
	EXPECT_EQ( ma[ j.at('e') ], 1.618);
	EXPECT_EQ( ma[ j.at('g') ], 0.693);
	EXPECT_EQ( ma[ j.at('i') ], 0.577);
    }

    TEST_F(MATest_MDim, SimpleCall)
    {
	MultiArray<double,MATest_MDim::MRange,MATest_MDim::SRange> ma(mrptr, sr3ptr, vv);
	EXPECT_EQ( ma.size(), 24u );
	EXPECT_EQ( ma.range()->dim(), 2u );

	auto i = ma.beginIndex();
	EXPECT_EQ( ma[ i.at( mkt( mkt('x', 'a'), '1' ) ) ], 2.917);
	EXPECT_EQ( ma[ i.at( mkt( mkt('x', 'a'), '2' ) ) ], 9.436);

	EXPECT_EQ( ma.at( mkt( mkt('x', 'a'), '1' ) ), 2.917);
	EXPECT_EQ( ma.at( mkt( mkt('x', 'a'), '2' ) ), 9.436);

	ma.at( mkt( mkt('x', 'a'), '2' ) ) = 4.444;
	EXPECT_EQ( ma[ i.at( mkt( mkt('x', 'a'), '2' ) ) ], 4.444 );
    }
    
    TEST_F(MATest_MDim, ReFormat)
    {
	MultiArray<double,MATest_MDim::MRange,MATest_MDim::SRange> ma(mrptr, sr3ptr, vv);

	auto ma2 = ma.format( sr4ptr );
	auto i = ma2.beginIndex();
	EXPECT_EQ( ma2.at('A') , 2.917 );
	EXPECT_EQ( ma2[ i.at('G') ], 4.892 );
	EXPECT_EQ( ma2.at('J') , 4.790 );
	EXPECT_EQ( ma2[ i.at('M') ], 1.530 );
	EXPECT_EQ( ma2.at('W') , 4.209 );
    }
    
    
} // end namespace 

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
