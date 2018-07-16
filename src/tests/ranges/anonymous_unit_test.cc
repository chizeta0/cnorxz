// -*- C++ -*-

#include <cstdlib>
#include "gtest/gtest.h"
#include <iostream>

#include "ranges/rheader.h"

//#include "multi_array_header.h"

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
    
    class AnonymousIndexTest : public ::testing::Test
    {
    protected:

	typedef SingleRangeFactory<char,SpaceType::ANY> SRF;
	typedef SRF::oType SRange;

	typedef MultiRangeFactory<SRange,SRange,SRange> M3RF;
	typedef M3RF::oType M3Range;

	typedef MultiRangeFactory<SRange,M3Range,SRange> MasterRF;
	typedef MasterRF::oType MasterRange;

	typedef ContainerRangeFactory<double,M3Range,SRange> CRF;
	typedef CRF::oType CRange;
	typedef ContainerIndex<double,M3Range::IndexType,SRange::IndexType> CIndex;

	AnonymousIndexTest()
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

	    swapMFactory<MasterRF>(rfbptr, sr1ptr, m3rptr, sr2ptr);
	    mstrptr = std::dynamic_pointer_cast<MasterRange>( rfbptr->create() );

	    swapMFactory<CRF>(rfbptr, m3rptr, sr2ptr);
	    cr1ptr = std::dynamic_pointer_cast<CRange>( rfbptr->create() );

	    swapMFactory<CRF>(rfbptr, m3rptr, sr1ptr);
	    cr2ptr = std::dynamic_pointer_cast<CRange>( rfbptr->create() );
	}

	std::shared_ptr<RangeFactoryBase> rfbptr;
	std::shared_ptr<SRange> sr1ptr;
	std::shared_ptr<SRange> sr2ptr;
	std::shared_ptr<M3Range> m3rptr;
	std::shared_ptr<MasterRange> mstrptr;
	std::shared_ptr<CRange> cr1ptr;
	std::shared_ptr<CRange> cr2ptr;
    };

    TEST_F(AnonymousIndexTest, Cast1)
    {
	AnonymousRangeFactory arf1(sr1ptr,m3rptr);
	auto ar1a = std::dynamic_pointer_cast<AnonymousRange>( arf1.create() );

	arf1.append(sr2ptr);
	
	auto ar1b = std::dynamic_pointer_cast<AnonymousRange>( arf1.create() );
	
	AnonymousRangeFactory arf2(sr1ptr,m3rptr,sr2ptr);
	auto ar2 = std::dynamic_pointer_cast<AnonymousRange>( arf2.create() );

	EXPECT_EQ(ar1b.get(), ar2.get());
	EXPECT_EQ(ar1a->size(), sr1ptr->size() * m3rptr->size());
	EXPECT_EQ(ar2->size(), sr1ptr->size() * m3rptr->size() * sr2ptr->size());

	EXPECT_EQ(ar1a->sub(0)->size(), sr1ptr->size());
	EXPECT_EQ(ar1a->sub(1)->size(), m3rptr->size());

	EXPECT_EQ(ar2->sub(0)->size(), sr1ptr->size());
	EXPECT_EQ(ar2->sub(1)->size(), m3rptr->size());
	EXPECT_EQ(ar2->sub(2)->size(), sr2ptr->size());
    }
    
} // end namespace 

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
