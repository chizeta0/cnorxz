// -*- C++ -*-

#include <cstdlib>
#include "gtest/gtest.h"
#include <iostream>

#include "multi_array_header.h"

namespace MAT = MultiArrayTools;

namespace {

    using namespace MAT;
    
    class ConnectionTest : public ::testing::Test
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

	ConnectionTest()
	{
	    rfbptr.swap( std::make_shared<SRF>( { 'e', 'b', 'n' } ) );
	    sr1ptr = std::dynamic_pointer_cast<SRange>( rfbptr.create() );

	    rfbptr.swap( std::make_shared<SRF>( { 'x', 'y', 'l', 'f' } ) );
	    sr2ptr = std::dynamic_pointer_cast<SRange>( rfbptr.create() );

	    rfbptr.swap( std::make_shared<SRF>( { 'a', 'b' } ) );
	    std::shared_ptr<SRange> temp1 = std::dynamic_pointer_cast<SRange>( rfbptr.create() );
	    rfbptr.swap( std::make_shared<SRF>( { '1' } ) );
	    std::shared_ptr<SRange> temp2 = std::dynamic_pointer_cast<SRange>( rfbptr.create() );
	    rfbptr.swap( std::make_shared<SRF>( { '0', '7' } ) );
	    std::shared_ptr<SRange> temp3 = std::dynamic_pointer_cast<SRange>( rfbptr.create() );

	    rfbptr.swap( std::make_shared<M3RF>( temp1, temp2, temp3 ) );
	    m3rptr = std::dynamic_pointer_cast<M3Range>( rfbptr.create() );
	}

	std::shared_ptr<RangeFactoryBase> rfbptr;
	std::shared_ptr<SRange> sr1ptr;
	std::shared_ptr<SRange> sr2ptr;
	std::shared_ptr<M3Range> m3rptr;
    };

    TEST_F(ConnectionTest, UnlinkedIteration)
    {
	EXPECT_EQ(i3d.pos(), 0);
    }
    
} // end namespace 

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
