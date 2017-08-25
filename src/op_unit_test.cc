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

    class OpTest_1Dim : public ::testing::Test
    {
    protected:

	typedef SingleRangeFactory<char,RangeType::ANY> SRF;
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

	typedef SingleRangeFactory<char,RangeType::ANY> SRF;
	typedef SRF::oType SRange;

	typedef MultiRangeFactory<SRange,SRange> MRF;
	typedef MRF::oType MRange;
	
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
    };

    TEST_F(OpTest_1Dim, ExecOp)
    {
	MultiArray<double,SRange> ma1(srptr, v1);
	MultiArray<double,SRange> ma2(srptr, v2);
	MultiArray<double,SRange> res(srptr);

	auto i = std::dynamic_pointer_cast<SRange::IndexType>( srptr->index() );
	res(i) = ma1(i) + ma2(i);

	EXPECT_EQ( fabs( res.at('a') - (2.917+8.870) ) < 0.0001, true);
	EXPECT_EQ( fabs( res.at('l') - (9.436+4.790) ) < 0.0001, true );
	EXPECT_EQ( fabs( res.at('f') - (0.373+8.215) ) < 0.0001, true );
	EXPECT_EQ( fabs( res.at('g') - (7.192+5.063) ) < 0.0001, true );
    }

    TEST_F(OpTest_MDim, ExecOp1)
    {
	MultiArray<double,SRange,SRange> res(sr2ptr,sr4ptr);
	MultiArray<double,SRange> ma1(sr2ptr, v1);
	MultiArray<double,SRange> ma2(sr4ptr, v2);

	auto i1 = std::dynamic_pointer_cast<SRange::IndexType>( sr2ptr->index() );
	auto i2 = std::dynamic_pointer_cast<SRange::IndexType>( sr4ptr->index() );
	
	res(i1,i2) = ma1(i1) + ma2(i2);

	EXPECT_EQ( fabs( res.at(mkt('1','A')) - (2.917 + 8.870) ) < 0.0001, true );
	EXPECT_EQ( fabs( res.at(mkt('1','B')) - (2.917 + 4.790) ) < 0.0001, true );

	EXPECT_EQ( fabs( res.at(mkt('2','A')) - (9.436 + 8.870) ) < 0.0001, true );
	EXPECT_EQ( fabs( res.at(mkt('2','B')) - (9.436 + 4.790) ) < 0.0001, true );

	EXPECT_EQ( fabs( res.at(mkt('3','A')) - (0.373 + 8.870) ) < 0.0001, true );
	EXPECT_EQ( fabs( res.at(mkt('3','B')) - (0.373 + 4.790) ) < 0.0001, true );

    }

    TEST_F(OpTest_MDim, ExecOp2)
    {
	MultiArray<double,MRange,SRange> res(mr1ptr,sr4ptr);
	MultiArray<double,MRange> ma1(mr1ptr, v3);
	MultiArray<double,SRange> ma2(sr4ptr, v2);

	auto i1 = std::dynamic_pointer_cast<MRange::IndexType>( mr1ptr->index() );
	auto i2 = std::dynamic_pointer_cast<SRange::IndexType>( sr4ptr->index() );
	
	res(i1,i2) = ma1(i1) + ma2(i2);

	EXPECT_EQ( fabs( res.at(mkt(mkt('1','a'),'A')) - (0.353 + 8.870) ) < 0.0001, true );
	EXPECT_EQ( fabs( res.at(mkt(mkt('1','a'),'B')) - (0.353 + 4.790) ) < 0.0001, true );
	EXPECT_EQ( fabs( res.at(mkt(mkt('1','b'),'A')) - (4.005 + 8.870) ) < 0.0001, true );
	EXPECT_EQ( fabs( res.at(mkt(mkt('1','b'),'B')) - (4.005 + 4.790) ) < 0.0001, true );

	EXPECT_EQ( fabs( res.at(mkt(mkt('2','a'),'A')) - (1.070 + 8.870) ) < 0.0001, true );
	EXPECT_EQ( fabs( res.at(mkt(mkt('2','a'),'B')) - (1.070 + 4.790) ) < 0.0001, true );
	EXPECT_EQ( fabs( res.at(mkt(mkt('2','b'),'A')) - (2.310 + 8.870) ) < 0.0001, true );
	EXPECT_EQ( fabs( res.at(mkt(mkt('2','b'),'B')) - (2.310 + 4.790) ) < 0.0001, true );

	EXPECT_EQ( fabs( res.at(mkt(mkt('3','a'),'A')) - (9.243 + 8.870) ) < 0.0001, true );
	EXPECT_EQ( fabs( res.at(mkt(mkt('3','a'),'B')) - (9.243 + 4.790) ) < 0.0001, true );
	EXPECT_EQ( fabs( res.at(mkt(mkt('3','b'),'A')) - (2.911 + 8.870) ) < 0.0001, true );
	EXPECT_EQ( fabs( res.at(mkt(mkt('3','b'),'B')) - (2.911 + 4.790) ) < 0.0001, true );

    }

    
} // anonymous namspace

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
