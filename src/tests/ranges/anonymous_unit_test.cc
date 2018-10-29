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
    
    class AnonymousTest : public ::testing::Test
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

	AnonymousTest()
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

    TEST_F(AnonymousTest, Cast1)
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

    TEST_F(AnonymousTest, Cast2)
    {
	AnonymousRangeFactory arf2(sr1ptr,m3rptr,sr2ptr);
	auto ar = std::dynamic_pointer_cast<AnonymousRange>( arf2.create() );
	auto mr = ar->template scast<SRange,AnonymousRange>(1,2);

	EXPECT_EQ(mr->template getPtr<0>()->size(), sr1ptr->size());
	EXPECT_EQ(mr->template getPtr<1>()->size(), m3rptr->size() * sr2ptr->size());
    }

    TEST_F(AnonymousTest, DCast1)
    {
	DynamicRangeFactory arf1(sr1ptr,m3rptr);
	auto ar1a = std::dynamic_pointer_cast<DynamicRange>( arf1.create() );

	arf1.append(sr2ptr);
	
	auto ar1b = std::dynamic_pointer_cast<DynamicRange>( arf1.create() );
	
	DynamicRangeFactory arf2(sr1ptr,m3rptr,sr2ptr);
	auto ar2 = std::dynamic_pointer_cast<DynamicRange>( arf2.create() );

	EXPECT_EQ(ar1b.get(), ar2.get());
	EXPECT_EQ(ar1a->size(), sr1ptr->size() * m3rptr->size());
	EXPECT_EQ(ar2->size(), sr1ptr->size() * m3rptr->size() * sr2ptr->size());

	EXPECT_EQ(ar1a->sub(0)->size(), sr1ptr->size());
	EXPECT_EQ(ar1a->sub(1)->size(), m3rptr->size());

	EXPECT_EQ(ar2->sub(0)->size(), sr1ptr->size());
	EXPECT_EQ(ar2->sub(1)->size(), m3rptr->size());
	EXPECT_EQ(ar2->sub(2)->size(), sr2ptr->size());
    }

    TEST_F(AnonymousTest, DCast2)
    {
	DynamicRangeFactory arf2(sr1ptr,m3rptr,sr2ptr);
	auto ar = std::dynamic_pointer_cast<DynamicRange>( arf2.create() );
	auto mr = ar->template scast<SRange,DynamicRange>(1,2);

	EXPECT_EQ(mr->template getPtr<0>()->size(), sr1ptr->size());
	EXPECT_EQ(mr->template getPtr<1>()->size(), m3rptr->size() * sr2ptr->size());
    }

    TEST_F(AnonymousTest, ToString1)
    {
	std::vector<char> vv = sr1ptr->data();
	const char* dp = vv.data();
	auto ff = createRangeFactory(&dp);
	auto ss = std::dynamic_pointer_cast<SRange>( ff->create() );

	EXPECT_EQ(sr1ptr->size(), ss->size());
	EXPECT_EQ(sr1ptr->get(0), ss->get(0));
	EXPECT_EQ(sr1ptr->get(1), ss->get(1));
	EXPECT_EQ(sr1ptr->get(2), ss->get(2));
    }

    TEST_F(AnonymousTest, ToString2)
    {
	typedef SingleRangeFactory<std::string,SpaceType::ANY> NameRF;
	typedef SingleRange<std::string,SpaceType::ANY> NameRange;
	ClassicRF crf(5);
	SpinRF srf;
	NameRF nrf({"ab", "cdef", "gh", "ijklmno"});
	
	auto cr = std::dynamic_pointer_cast<ClassicRange>( crf.create() );
	auto sr = std::dynamic_pointer_cast<SpinRange>( srf.create() );
	auto nr = std::dynamic_pointer_cast<NameRange>( nrf.create() );
	
	std::vector<char> cv = cr->data();
	std::vector<char> sv = sr->data();
	std::vector<char> nv = nr->data(); 
	const char* cp = cv.data();
	const char* sp = sv.data();
	const char* np = nv.data();

	auto crf2 = createRangeFactory(&cp);
	auto cr2 = std::dynamic_pointer_cast<ClassicRange>( crf2->create() );

	auto srf2 = createRangeFactory(&sp);
	auto sr2 = std::dynamic_pointer_cast<SpinRange>( srf2->create() );

	auto nrf2 = createRangeFactory(&np);
	auto nr2 = std::dynamic_pointer_cast<NameRange>( nrf2->create() );

	EXPECT_EQ(cr2->size(), cr->size());
	EXPECT_EQ(sr2->size(), sr->size());

	EXPECT_EQ(nr2->size(), nr->size());
	EXPECT_EQ(nr2->get(0), nr->get(0));
	EXPECT_EQ(nr2->get(1), nr->get(1));
	EXPECT_EQ(nr2->get(2), nr->get(2));
	EXPECT_EQ(nr2->get(3), nr->get(3));
    }
    

    TEST_F(AnonymousTest, ToStringMulti)
    {
	std::vector<char> vv = m3rptr->data();
	const char* dp = vv.data();
	
	auto ff2 = std::dynamic_pointer_cast<M3RF>( createRangeFactory(&dp) );
	auto mr2 = std::dynamic_pointer_cast<M3Range>( ff2->create() );

	EXPECT_EQ(mr2->size(), m3rptr->size());
	EXPECT_EQ(mr2->template getPtr<0>()->size(),
		  m3rptr->template getPtr<0>()->size());
	EXPECT_EQ(mr2->template getPtr<1>()->size(),
		  m3rptr->template getPtr<1>()->size());
	EXPECT_EQ(mr2->template getPtr<2>()->size(),
		  m3rptr->template getPtr<2>()->size());
	auto jj = m3rptr->begin();
	for(auto ii = mr2->begin(); ii.pos() != ii.max(); ++ii, jj = ii.pos()){
	    EXPECT_EQ(ii.meta(), jj.meta());
	}
    }

    TEST_F(AnonymousTest, ToStringAnonymous)
    {
	AnonymousRangeFactory arf2(sr1ptr,m3rptr,sr2ptr);
	auto ar = std::dynamic_pointer_cast<AnonymousRange>( arf2.create() );
	std::vector<char> vv = ar->data();
	const char* dp = vv.data();
	
	auto ff2 = std::dynamic_pointer_cast<AnonymousRangeFactory>( createRangeFactory(&dp) );
	auto ar2 = std::dynamic_pointer_cast<AnonymousRange>( ff2->create() );

	EXPECT_EQ(ar2->size(), ar->size());
	EXPECT_EQ(ar2->anonymousDim(), ar->anonymousDim());
    }

} // end namespace 

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
