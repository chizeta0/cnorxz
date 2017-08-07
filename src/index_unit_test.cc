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
    
    class IndexTest : public ::testing::Test
    {
    protected:

	typedef SingleRangeFactory<char,RangeType::ANY> SRF;
	typedef SRF::oType SRange;

	typedef MultiRangeFactory<SRange,SRange,SRange> M3RF;
	typedef M3RF::oType M3Range;

	typedef MultiRangeFactory<SRange,M3Range,SRange> MasterRF;
	typedef MasterRF::oType MasterRange;

	typedef ContainerRangeFactory<M3Range,SRange> CRF;
	typedef CRF::oType CRange;

	IndexTest()
	{
	    swapFactory<SRF>(rfbptr, { 'e', 'b', 'n' } );
	    sr1ptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

	    swapFactory<SRF>(rfbptr, { 'x', 'y', 'l', 'f' } );
	    sr2ptr = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

	    swapFactory<SRF>(rfbptr, { 'a', 'b' } );
	    std::shared_ptr<SRange> temp1 = std::dynamic_pointer_cast<SRange>( rfbptr->create() );
	    swapFactory<SRF>(rfbptr, { '1' } );
	    std::shared_ptr<SRange> temp2 = std::dynamic_pointer_cast<SRange>( rfbptr->create() );
	    swapFactory<SRF>(rfbptr, { '0', '7' } );
	    std::shared_ptr<SRange> temp3 = std::dynamic_pointer_cast<SRange>( rfbptr->create() );

	    swapMFactory<M3RF>(rfbptr, temp1, temp2, temp3 );
	    m3rptr = std::dynamic_pointer_cast<M3Range>( rfbptr->create() );
	}

	std::shared_ptr<RangeFactoryBase> rfbptr;
	std::shared_ptr<SRange> sr1ptr;
	std::shared_ptr<SRange> sr2ptr;
	std::shared_ptr<M3Range> m3rptr;
    };

    TEST_F(IndexTest, SingleIndex_SimpleCall)
    {
	auto si = sr1ptr->begin();
	EXPECT_EQ(si.max(), 3);
	EXPECT_EQ(si.pos(), 0);
	EXPECT_EQ(si.first(), true);
	EXPECT_EQ(si.last(), false);
	EXPECT_EQ(si.meta(), 'e');
	si.at('n');
	EXPECT_EQ(si.pos(), si.max()-1);
	EXPECT_EQ(si.first(), false);
	EXPECT_EQ(si.last(), true);
	si = 1;
	EXPECT_EQ(si.meta(), 'b');
	++si;
	EXPECT_EQ(si.meta(), 'n');
	auto si2 = sr1ptr->end();
	--si2;
	EXPECT_EQ(si == si2, true);
	EXPECT_EQ(si != si2, false);
	auto si3 = sr2ptr->end();
	--si3;
	EXPECT_EQ(si == si3, false);
	EXPECT_EQ(si != si3, true);
    }

    TEST_F(IndexTest, MultiIndex_SimpleCall)
    {
	auto mi = m3rptr->begin();
	EXPECT_EQ(mi.max(), 4);
	EXPECT_EQ(mi.pos(), 0);
	EXPECT_EQ(mi.first(), true);
	EXPECT_EQ(mi.last(), false);
	EXPECT_EQ(mi.meta() == std::make_tuple('a','1','0'), true);
	mi.at(std::make_tuple('b','1','7'));
	EXPECT_EQ(mi.pos(), mi.max()-1);
	EXPECT_EQ(mi.first(), false);
	EXPECT_EQ(mi.last(), true);
	mi = 1;
	EXPECT_EQ(mi.meta() == std::make_tuple('a','1','7'), true);
	++mi;
	EXPECT_EQ(mi.meta() == std::make_tuple('b','1','0'), true);
	++mi;
	auto mi2 = m3rptr->end();
	--mi2;
	EXPECT_EQ(mi == mi2, true);
	EXPECT_EQ(mi != mi2, false);
	--mi2;
	EXPECT_EQ(mi == mi2, false);
	EXPECT_EQ(mi != mi2, true);
    }

} // end namespace 

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
