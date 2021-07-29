// -*- C++ -*-

#include <cstdlib>
#include "gtest/gtest.h"
#include <iostream>

#include "cnorxz.h"

namespace MAT = CNORXZ;

namespace {

    using namespace MAT;

    template <class Factory, typename T>
    void swapFactory(std::shared_ptr<RangeFactoryBase>& fptr, std::initializer_list<T> ilist)
    {
	vector<T> tmp = ilist;
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
	vector<double> vv = { 3.141, 2.718, 1.618, 0.693, 0.577 };
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
	vector<double> vv = { 2.917, 9.436, 0.373, 7.192, 7.315, 1.536, 4.892, 0.280,
				   8.870, 4.790, 8.215, 5.063, 1.530, 3.084, 1.609, 4.847,
				   8.175, 0.112, 6.712, 6.408, 1.959, 0.331, 4.209, 2.951 };
    };

    TEST_F(MATest_1Dim, SimpleCall)
    {
	Array<double,MATest_1Dim::SRange> ma(srptr, vv);
	EXPECT_EQ( ma.size(), 5u);
	EXPECT_EQ( ma.isConst(), false);
	EXPECT_EQ( ma.isSlice(), false);	
	
	auto i = ma.cbegin();
	EXPECT_EQ( ma[ i.at('x') ], 3.141);
	EXPECT_EQ( ma[ i.at('y') ], 2.718);
	EXPECT_EQ( ma[ i.at('l') ], 1.618);
	EXPECT_EQ( ma[ i.at('f') ], 0.693);
	EXPECT_EQ( ma[ i.at('g') ], 0.577);

    }
    
    TEST_F(MATest_1Dim, ForLoop)
    {
	vector<double> v2 = { 0.693 , 2.718, 3.141, 1.618, 9.98 };
	Array<double,MATest_1Dim::SRange> ma(srptr, std::move( v2 ) );
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
	
	Array<double,MATest_1Dim::SRange> ma(srptr, vv);
	auto i = ma.cbegin();
	EXPECT_EQ( ma[ i.at('x') ], 3.141);

	auto ma2 = ma.format( sr2 );
	auto j = ma2.cbegin();
	
	EXPECT_EQ( ma[ j.at('a') ], 3.141);
	EXPECT_EQ( ma[ j.at('c') ], 2.718);
	EXPECT_EQ( ma[ j.at('e') ], 1.618);
	EXPECT_EQ( ma[ j.at('g') ], 0.693);
	EXPECT_EQ( ma[ j.at('i') ], 0.577);
    }

    TEST_F(MATest_MDim, SimpleCall)
    {
	Array<double,MATest_MDim::MRange,MATest_MDim::SRange> ma(mrptr, sr3ptr, vv);
	EXPECT_EQ( ma.size(), 24u );
	EXPECT_EQ( ma.range()->dim(), 2u );

	auto i = ma.cbegin();
	EXPECT_EQ( ma[ i.at( mkt( mkt('x', 'a'), '1' ) ) ], 2.917);
	EXPECT_EQ( ma[ i.at( mkt( mkt('x', 'a'), '2' ) ) ], 9.436);

	EXPECT_EQ( ma.at( mkt( mkt('x', 'a'), '1' ) ), 2.917);
	EXPECT_EQ( ma.at( mkt( mkt('x', 'a'), '2' ) ), 9.436);

	ma.at( mkt( mkt('x', 'a'), '2' ) ) = 4.444;
	EXPECT_EQ( ma[ i.at( mkt( mkt('x', 'a'), '2' ) ) ], 4.444 );
    }
    
    TEST_F(MATest_MDim, ReFormat)
    {
	Array<double,MATest_MDim::MRange,MATest_MDim::SRange> ma(mrptr, sr3ptr, vv);

	auto ma2 = ma.format( sr4ptr );
	auto i = ma2.cbegin();
	EXPECT_EQ( ma2.at('A') , 2.917 );
	EXPECT_EQ( ma2[ i.at('G') ], 4.892 );
	EXPECT_EQ( ma2.at('J') , 4.790 );
	EXPECT_EQ( ma2[ i.at('M') ], 1.530 );
	EXPECT_EQ( ma2.at('W') , 4.209 );
    }

    TEST_F(MATest_MDim, SliceTest1)
    {
	Array<double,MATest_MDim::MRange,MATest_MDim::SRange> ma(mrptr, sr3ptr, vv);
	Slice<double,MATest_MDim::SRange> sl(sr3ptr);

	auto i = MAT::getIndex(sr3ptr);
	auto mi = MAT::getIndex(mrptr);
	mi->at( mkt('y', 'f') );
	sl.define(i) = ma(mi, i);

	EXPECT_EQ( sl.at('1'), 6.712 );
	EXPECT_EQ( sl.at('2'), 6.408 );
	EXPECT_EQ( sl.at('3'), 1.959 );
	
	Slice<double,SRange> sl2(sr2ptr);
	auto j = MAT::getIndex(sr3ptr);
	auto mj = MAT::getIndex(mrptr);
	mj->at( mkt('y','a') );
	j->at('2');
	auto jj = mj->template getPtr<1>();
	sl2.define(jj) = ma(mj, j);

	EXPECT_EQ( sl2.at('a'), 3.084 );
	EXPECT_EQ( sl2.at('l'), 8.175 );
	EXPECT_EQ( sl2.at('f'), 6.408 );
	EXPECT_EQ( sl2.at('g'), 4.209 );

	Slice<double,SRange,SRange,SRange> sl3(sr3ptr, sr1ptr, sr2ptr);
	mj->at( mkt('x','a') );
	auto j1 = MAT::getIndex(sr1ptr);
	auto j2 = MAT::getIndex(sr2ptr);
	(*mj)(j1,j2);
	j->at('1');
	sl3.define(j, j1, j2) = ma(mj, j);

	auto ci = sl3.begin();

	EXPECT_EQ( sl3.at( mkt('1', 'x','a') ), 2.917 );
	EXPECT_EQ( *ci, 2.917 );
	EXPECT_EQ( sl3.at( mkt('2', 'x','a') ), 9.436 );
	EXPECT_EQ( sl3.at( mkt('3', 'x','a') ), 0.373 );

	EXPECT_EQ( sl3.at( mkt('1', 'x','l') ), 7.192 );
	EXPECT_EQ( *(++ci), 7.192 );
	EXPECT_EQ( sl3.at( mkt('2', 'x','l') ), 7.315 );
	EXPECT_EQ( sl3.at( mkt('3', 'x','l') ), 1.536 );

	EXPECT_EQ( sl3.at( mkt('1', 'x','f') ), 4.892 );
	EXPECT_EQ( *(++ci), 4.892 );
	EXPECT_EQ( sl3.at( mkt('2', 'x','f') ), 0.280 );
	EXPECT_EQ( sl3.at( mkt('3', 'x','f') ), 8.870 );

	EXPECT_EQ( sl3.at( mkt('1', 'x','g') ), 4.790 );
	EXPECT_EQ( *(++ci), 4.790 );
	EXPECT_EQ( sl3.at( mkt('2', 'x','g') ), 8.215 );
	EXPECT_EQ( sl3.at( mkt('3', 'x','g') ), 5.063 );
	
	EXPECT_EQ( sl3.at( mkt('1', 'y','a') ), 1.530 );
	EXPECT_EQ( *(++ci), 1.530 );
	EXPECT_EQ( sl3.at( mkt('2', 'y','a') ), 3.084 );
	EXPECT_EQ( sl3.at( mkt('3', 'y','a') ), 1.609 );
	
	EXPECT_EQ( sl3.at( mkt('1', 'y','l') ), 4.847 );
	EXPECT_EQ( *(++ci), 4.847 );
	EXPECT_EQ( sl3.at( mkt('2', 'y','l') ), 8.175 );
	EXPECT_EQ( sl3.at( mkt('3', 'y','l') ), 0.112 );

	EXPECT_EQ( sl3.at( mkt('1', 'y','f') ), 6.712 );
	EXPECT_EQ( *(++ci), 6.712 );
	EXPECT_EQ( sl3.at( mkt('2', 'y','f') ), 6.408 );
	EXPECT_EQ( sl3.at( mkt('3', 'y','f') ), 1.959 );

	EXPECT_EQ( sl3.at( mkt('1', 'y','g') ), 0.331 );
	EXPECT_EQ( *(++ci), 0.331 );
	EXPECT_EQ( sl3.at( mkt('2', 'y','g') ), 4.209 );
	EXPECT_EQ( sl3.at( mkt('3', 'y','g') ), 2.951 );

	EXPECT_EQ( *(++ci), 9.436 );
	EXPECT_EQ( *(++ci), 7.315 );
	
    }
    
} // end namespace 

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
