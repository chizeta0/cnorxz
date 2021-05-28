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

    class DynIndexTest : public ::testing::Test
    {
    protected:
        typedef DynamicRangeFactory DRF;
        typedef DRF::oType DR;

	typedef SingleRangeFactory<char,SpaceType::ANY> SRF;
	typedef SRF::oType SRange;

	typedef MultiRangeFactory<SRange,SRange,SRange> M3RF;
	typedef M3RF::oType M3Range;

        DynIndexTest()
        {
            SRF srf({'e', 'b', 'n'});
            sr = std::dynamic_pointer_cast<SRange>( srf.create() );

            SRF srf1({'a', 'b'});
            SRF srf2({'1'});
            SRF srf3({'0', '7'});
            auto sr1 = std::dynamic_pointer_cast<SRange>( srf1.create() );
            auto sr2 = std::dynamic_pointer_cast<SRange>( srf2.create() );
            auto sr3 = std::dynamic_pointer_cast<SRange>( srf3.create() );
            M3RF mrf(sr1,sr2,sr3);
            mr = std::dynamic_pointer_cast<M3Range>( mrf.create() );

            vector<std::shared_ptr<RangeBase>> rv(2);
            rv[0] = sr;
            rv[1] = mr;
            DRF drf(rv);
            dr = std::dynamic_pointer_cast<DR>( drf.create() );
        }

        std::shared_ptr<DR> dr;
        std::shared_ptr<SRange> sr;
        std::shared_ptr<M3Range> mr;
    };
    
    class IndexTest : public ::testing::Test
    {
    protected:

	typedef SingleRangeFactory<char,SpaceType::ANY> SRF;
	typedef SRF::oType SRange;

	typedef MultiRangeFactory<SRange,SRange,SRange> M3RF;
	typedef M3RF::oType M3Range;

	typedef MultiRangeFactory<SRange,M3Range,SRange> MasterRF;
	typedef MasterRF::oType MasterRange;

	typedef ContainerRangeFactory<M3Range,SRange> CRF;
	typedef CRF::oType CRange;
	typedef ConstContainerIndex<double,M3Range::IndexType,SRange::IndexType> CIndex;

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

    TEST_F(IndexTest, SingleIndex_SimpleCall)
    {
	auto si = sr1ptr->begin();
	EXPECT_EQ(si.max(), 3u);
	EXPECT_EQ(si.pos(), 0u);
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
	EXPECT_EQ(mi.max(), 4u);
	EXPECT_EQ(mi.pos(), 0u);
	EXPECT_EQ(mi.first(), true);
	EXPECT_EQ(mi.last(), false);
	EXPECT_EQ(mi.meta() == mkt('a','1','0'), true);
	mi.at( mkt('b','1','7') );
	EXPECT_EQ(mi.pos(), mi.max()-1);
	EXPECT_EQ(mi.first(), false);
	EXPECT_EQ(mi.last(), true);
	mi = 1;
	EXPECT_EQ(mi.meta() == mkt('a','1','7'), true);
	++mi;
	EXPECT_EQ(mi.meta() == mkt('b','1','0'), true);
	++mi;
	auto mi2 = m3rptr->end();
	--mi2;
	EXPECT_EQ(mi == mi2, true);
	EXPECT_EQ(mi != mi2, false);
	--mi2;
	EXPECT_EQ(mi == mi2, false);
	EXPECT_EQ(mi != mi2, true);
	auto si = sr1ptr->begin();
	EXPECT_EQ(si == mi, false);
	EXPECT_EQ(si != mi, true);
    }

    TEST_F(IndexTest, MasterRange_Check)
    {
	EXPECT_EQ(mstrptr->size(), 48u);
	EXPECT_EQ(mstrptr->template get<0>().size(), 3u);
	EXPECT_EQ(mstrptr->template get<1>().size(), 4u);
	EXPECT_EQ(mstrptr->template get<2>().size(), 4u);
	EXPECT_EQ(mstrptr->dim(), 3u);
	EXPECT_EQ(mstrptr->template get<0>().dim(), 1u);
	EXPECT_EQ(mstrptr->template get<1>().dim(), 3u);

	auto mi = mstrptr->begin();
	EXPECT_EQ(mi.meta() == mkt( 'e' , mkt('a', '1', '0') , 'x' ), true);
	mi = mi.max()-1;
	EXPECT_EQ(mi.meta() == mkt( 'n' , mkt('b', '1', '7') , 'f' ), true);
	mi.template down<1>();
	EXPECT_EQ(mi.meta() == mkt( 'n' , mkt('b', '1', '0') , 'f' ), true);
	mi.template down<0>();
	EXPECT_EQ(mi.meta() == mkt( 'b' , mkt('b', '1', '0') , 'f' ), true);
	mi.template down<2>();
	EXPECT_EQ(mi.meta() == mkt( 'b' , mkt('b', '1', '0') , 'l' ), true);
	mi.template up<1>();
	EXPECT_EQ(mi.meta() == mkt( 'b' , mkt('b', '1', '7') , 'l' ), true);

	auto& subI = mi.template get<0>();
	EXPECT_EQ(subI.meta(), 'b');
	mi.template up<0>();
	EXPECT_EQ(subI.meta(), 'n');
	auto& subI2 = mi.template get<1>();
	EXPECT_EQ(subI2.meta() == mkt('b', '1', '7'), true);
	mi.template down<1>();
	EXPECT_EQ(subI2.meta() == mkt('b', '1', '0'), true);
    }

    TEST_F(IndexTest, ContainerRange_Check)
    {
	EXPECT_EQ(cr1ptr->size(), 16u);
	EXPECT_EQ(cr2ptr->size(), 12u);

	auto mi = mstrptr->begin();
	//auto ci1 = cr1ptr->begin();
	CIndex ci1(cr1ptr, 0);
	ci1 = 0;
	//auto ci2 = cr2ptr->begin();
	CIndex ci2(cr2ptr, 0);
	ci2 = 0;
	
	EXPECT_EQ(ci1.max(), 16u);
	EXPECT_EQ(ci2.max(), 12u);
	
	ci1(mi.template getPtr<1>(), mi.template getPtr<2>());
	ci2(mi.template getPtr<1>(), mi.template getPtr<0>());

	EXPECT_EQ(ci1.pos(), 0u);
	EXPECT_EQ(ci2.pos(), 0u);
	++mi;
	EXPECT_EQ(ci1().pos(), 1u);
	EXPECT_EQ(ci2().pos(), 0u);
	mi.template up<1>();
	EXPECT_EQ(ci1().pos(), 5u);
	EXPECT_EQ(ci2().pos(), 3u);
	mi.template up<0>();
	EXPECT_EQ(ci1().pos(), 5u);
	EXPECT_EQ(ci2().pos(), 4u);
	mi = mi.max()-1;
	EXPECT_EQ(ci1().pos(), ci1.max()-1);
	EXPECT_EQ(ci2().pos(), ci2.max()-1);
    }

    const char& resolve0(const vector<char>& cv)
    {
        return cv[0];
    }

    const std::tuple<char,char,char>& resolve1(const vector<char>& cv)
    {
        return *reinterpret_cast<const std::tuple<char,char,char>*>(cv.data()+1);
    }

    TEST_F(DynIndexTest, Iterate)
    {
        for(auto i = dr->begin(); i.pos() != i.max(); ++i){
            EXPECT_EQ(i.meta().size(), 4*sizeof(char));
        }
        auto j = dr->begin();
        //std::cout << j.stringMeta() << std::endl;
        EXPECT_EQ(j.stringMeta(), "[e, [a,1,0]]");
        EXPECT_EQ(resolve0(j.meta()), 'e');
        EXPECT_EQ(resolve1(j.meta()), mkt('a','1','0'));

        EXPECT_EQ((++j).stringMeta(), "[e, [a,1,7]]");
        EXPECT_EQ(resolve0(j.meta()), 'e');
        EXPECT_EQ(resolve1(j.meta()), mkt('a','1','7'));

        EXPECT_EQ((++j).stringMeta(), "[e, [b,1,0]]");
        EXPECT_EQ(resolve0(j.meta()), 'e');
        EXPECT_EQ(resolve1(j.meta()), mkt('b','1','0'));

        EXPECT_EQ((++j).stringMeta(), "[e, [b,1,7]]");
        EXPECT_EQ(resolve0(j.meta()), 'e');
        EXPECT_EQ(resolve1(j.meta()), mkt('b','1','7'));


        EXPECT_EQ((++j).stringMeta(), "[b, [a,1,0]]");
        EXPECT_EQ(resolve0(j.meta()), 'b');
        EXPECT_EQ(resolve1(j.meta()), mkt('a','1','0'));

        EXPECT_EQ((++j).stringMeta(), "[b, [a,1,7]]");
        EXPECT_EQ(resolve0(j.meta()), 'b');
        EXPECT_EQ(resolve1(j.meta()), mkt('a','1','7'));

        EXPECT_EQ((++j).stringMeta(), "[b, [b,1,0]]");
        EXPECT_EQ(resolve0(j.meta()), 'b');
        EXPECT_EQ(resolve1(j.meta()), mkt('b','1','0'));

        EXPECT_EQ((++j).stringMeta(), "[b, [b,1,7]]");
        EXPECT_EQ(resolve0(j.meta()), 'b');
        EXPECT_EQ(resolve1(j.meta()), mkt('b','1','7'));


        EXPECT_EQ((++j).stringMeta(), "[n, [a,1,0]]");
        EXPECT_EQ(resolve0(j.meta()), 'n');
        EXPECT_EQ(resolve1(j.meta()), mkt('a','1','0'));

        EXPECT_EQ((++j).stringMeta(), "[n, [a,1,7]]");
        EXPECT_EQ(resolve0(j.meta()), 'n');
        EXPECT_EQ(resolve1(j.meta()), mkt('a','1','7'));

        EXPECT_EQ((++j).stringMeta(), "[n, [b,1,0]]");
        EXPECT_EQ(resolve0(j.meta()), 'n');
        EXPECT_EQ(resolve1(j.meta()), mkt('b','1','0'));

        EXPECT_EQ((++j).stringMeta(), "[n, [b,1,7]]");
        EXPECT_EQ(resolve0(j.meta()), 'n');
        EXPECT_EQ(resolve1(j.meta()), mkt('b','1','7'));

    }
    
} // end namespace 

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
