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

	typedef SingleRangeFactory<char,RangeType::ANY> SRF;
	typedef SRF::oType SRange;

	typedef ContainerRangeFactory<SRange> CRF;
	typedef CRF::oType CRange;

	MATest_1Dim()
	{
	    swapFactory<SRF>(rfbptr, {'x', 'y', 'l', 'f', 'g'} );
	    srptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

	    swapMFactory<CRF>(rfbptr, srptr);
	    crptr = std::dynamic_pointer_cast<CRange>( rfbptr->create() );
	}

	std::shared_ptr<RangeFactoryBase> rfbptr;
	std::shared_ptr<SRange> srptr;
	std::shared_ptr<CRange> crptr;
	std::vector<double> vv = { 3.141, 2.718, 1.618, 0.693, 0.577 };
    };

    /*
    class MATest_1Dim : public ::testing::Test
    {
    protected:

	typedef SingleRangeFactory<char,RangeType::ANY> SRF;
	typedef SRF::oType SRange;

	typedef ContainerRangeFactory<SRange> CRF;
	typedef CRF::oType CRange;

	MATest_1Dim()
	{
	    swapFactory<SRF>(rfbptr, {'x', 'y', 'l', 'f', 'g'} );
	    srptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

	    swapMFactory<CRF>(rfbptr, srptr);
	    crptr = std::dynamic_pointer_cast<CRange>( rfbptr->create() );
	}
	
	std::shared_ptr<RangeFactoryBase> rfbptr;
	std::shared_ptr<SRange> srptr;
	std::shared_ptr<CRange> crptr;
	std::vector<double> vv = { 3.141, 2.718, 1.618, 0.693, 0.577 };
	};*/
    
    TEST_F(MATest_1Dim, SimpleCall)
    {
	MultiArray<double,MATest_1Dim::CRange> ma(crptr, vv);
	EXPECT_EQ( ma.size(), 5);
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
	MultiArray<double,MATest_1Dim::CRange> ma(crptr, std::move( v2 ) );
	size_t cnt = 0;
	for(auto el: ma){

	    if(cnt == 0){
		EXPECT_EQ(el, 0.693);
	    } else if(cnt == 1){
		EXPECT_EQ(el, 2.718);
	    } else if(cnt == 2){
		EXPECT_EQ(el, 3.141);
	    } else if(cnt == 3){
		EXPECT_EQ(el, 1.618);
	    } else if(cnt == 4){
		EXPECT_EQ(el, 9.98);
	    } else {
		EXPECT_EQ(cnt, 0);
	    }
	    ++cnt;
	}
	EXPECT_EQ(cnt, ma.size());
    }

    TEST_F(MATest_1Dim, ReFormat)
    {
	swapFactory<SRF>( rfbptr, { 'a', 'c', 'e', 'g', 'i' } );
	std::shared_ptr<SRange> sr2 = std::dynamic_pointer_cast<SRange>( rfbptr->create() );
	swapMFactory<CRF>( rfbptr, sr2 );
	std::shared_ptr<CRange> cr2 = std::dynamic_pointer_cast<CRange>( rfbptr->create() );
	
	MultiArray<double,MATest_1Dim::CRange> ma(crptr, vv);
	auto i = ma.beginIndex();
	EXPECT_EQ( ma[ i.at('x') ], 3.141);

	auto ma2 = ma.format( cr2 );
	auto j = ma2.beginIndex();
	
	EXPECT_EQ( ma[ j.at('a') ], 3.141);
	EXPECT_EQ( ma[ j.at('c') ], 2.718);
	EXPECT_EQ( ma[ j.at('e') ], 1.618);
	EXPECT_EQ( ma[ j.at('g') ], 0.693);
	EXPECT_EQ( ma[ j.at('i') ], 0.577);
    }
    
    
} // end namespace 

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
