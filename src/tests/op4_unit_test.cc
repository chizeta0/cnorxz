
#include "test_header.h"
#include <ctime>

namespace
{

    template <class MA>
    void setMARandom(MA& ma, int seed)
    {
	std::srand(seed);
	double n = 0;
	double d = 0;
	for(auto& x: ma.vdata()){
	    double nx = 0;
	    double dx = 0;
	    while(nx == x or dx == d or nx == dx or dx == 0){
		nx = static_cast<double>(std::rand() % 1000);
		dx = static_cast<double>(std::rand() % 1000);
	    }
	    n = nx;
	    d = dx;
	    assert(n != d);
	    x = n/d;
	}
    }

    template <class Op>
    DynamicO<double> mkDynOp(const Op& op)
    {
	return DynamicO<double>(op);
    }

    template <class Op1, class Op2>
    DynamicO<double> mkDynMul(const Op1& op1, const Op2& op2)
    {
	auto op = op1 * op2;
	return DynamicO<double>(std::make_shared<DynamicOperation<double,decltype(op)>>(op));
    }
    
    class OpTest_Dyn : public ::testing::Test
    {
    protected:

	MultiArray<double,CR,DR> ma1;
	MultiArray<double,CR,DR> ma2;
	MultiArray<double,DR> ma3;
	MultiArray<double,DR> ma5;

	MultiArray<double,CR,DR> res1;
	MultiArray<double,CR,DR> res2;

	std::map<std::string,std::shared_ptr<IndexW>> imap;

	std::shared_ptr<DR> dr1;
	//std::shared_ptr<DR> dr1a;
	std::shared_ptr<DR> dr2;
	std::shared_ptr<DR> dr3;
	std::shared_ptr<DR> dr4;
	std::shared_ptr<DR> dr4a;
	std::shared_ptr<DR> dr5;
	std::shared_ptr<DR> dr6;
	std::shared_ptr<DR> dr6a;
	std::shared_ptr<CR> cr1;

	std::shared_ptr<CR::IndexType> ci4_1;
	std::shared_ptr<CR::IndexType> ci4_2;

	typedef CR::IndexType CI;
	
	OpTest_Dyn()
	{
	    cr1 = createRangeE<CR>(5);
	    
	    auto cr2 = createRangeE<CR>(7);
	    //auto cr2 = createRangeE<CR>(2);
	    auto cr3 = createRangeE<CR>(11);
	    auto cr4 = createRangeE<CR>(3);
	    auto cr5 = createRangeE<CR>(13);
	    //auto cr5 = createRangeE<CR>(1);

	    dr1 = createRangeE<DR>(cr2,cr2,cr3,cr4);
	    //dr1a = createRangeE<DR>(cr2,cr2,cr3);
	    dr2 = createRangeE<DR>(cr3,cr3,cr4);
	    dr3 = createRangeE<DR>(cr2,cr5);
	    dr5 = createRangeE<DR>(cr5);
	    dr6 = createRangeE<DR>(cr3,cr4);
	    dr6a = createRangeE<DR>(cr3,cr2,cr5);

	    dr4 = createRangeE<DR>(cr2,cr3,cr4,cr4);
	    dr4a = createRangeE<DR>(cr2,cr3);

	    ma1 = mkArray<double>(cr1,dr1);
	    ma2 = mkArray<double>(cr1,dr2);
	    ma3 = mkArray<double>(dr3);
	    ma5 = mkArray<double>(dr5);

	    res1 = mkArray<double>(cr1,dr4);
	    res2 = mkArray<double>(cr1,dr6);

	    setMARandom(ma1, 25);
	    setMARandom(ma2, 31);
	    setMARandom(ma3, 47);
	    setMARandom(ma5, 59);

	    imap["i2_1"] = mkIndexW(getIndex(cr2));
	    imap["i2_2"] = mkIndexW(getIndex(cr2));
	    imap["i3_1"] = mkIndexW(getIndex(cr3));
	    imap["i3_2"] = mkIndexW(getIndex(cr3));
	    ci4_1 = getIndex(cr4);
	    ci4_2 = getIndex(cr4);
	    imap["i4_1"] = mkIndexW(ci4_1);
	    imap["i4_2"] = mkIndexW(ci4_2);
	    imap["i5_1"] = mkIndexW(getIndex(cr5));
	    imap["i5_2"] = mkIndexW(getIndex(cr5));
	}
    };

    TEST_F(OpTest_Dyn, Multiply)
    {
	auto i1 = getIndex(cr1);
	auto di1 = getIndex(dr1);
	//auto di1a = getIndex(dr1a);
	auto di2 = getIndex(dr2);
	auto di4 = getIndex(dr4);
	auto di4a = getIndex(dr4a);
	
	(*di1)({imap["i2_1"],imap["i2_2"],imap["i3_1"],imap["i4_1"]});
	//(*di1a)({imap["i2_1"],imap["i2_2"],imap["i3_1"]});
	(*di2)({imap["i3_1"],imap["i3_1"],imap["i4_2"]});
	(*di4)({imap["i2_1"],imap["i3_1"],imap["i4_1"],imap["i4_2"]});
	(*di4a)({imap["i2_1"],imap["i3_1"]});

	auto mi = mkMIndex(i1,di4a);

	auto resx1 = res1;
	auto resx2 = res1;
	auto resx3 = res1;
	auto resx4 = res1;
	auto xx = std::make_shared<decltype(resx4)>(resx4);
	res1(i1,di4) = ma1(i1,di1) * ma2(i1,di2);
	resx1(i1,di4) = mkDynOp(ma1(i1,di1)) * mkDynOp(ma2(i1,di2));
        resx2(i1,di4) = mkDynOp(ma1(i1,di1) * ma2(i1,di2));
	resx3(i1,di4) = mkDynOp(mkDynOp(ma1(i1,di1)) * mkDynOp(ma2(i1,di2)));

	auto op1x = mkDynOutOp((ma1(i1,di1) * ma2(i1,di2)), ci4_1, ci4_2);
	auto opr = resx4(i1,di4);
	
	auto loop = mkPILoop
	    ( [&op1x,&opr,&xx,this](){
		auto op1 = op1x;
		return mkGetExpr(op1,mkILoop(std::make_tuple(opr,op1,*op1.data()->mOp), std::make_tuple(ci4_1, ci4_2),
					     std::make_tuple(xx),
					     std::make_tuple(opr.assign( *op1.data()->mOp, mkMIndex(ci4_1, ci4_2) )),
					     std::array<size_t,1>({1}), std::array<size_t,1>({0}))); } );

	//loop.dummy();
        mi->pifor(1,loop)();
	
	auto i2_1 = imap.at("i2_1");
	auto i2_2 = imap.at("i2_2");
	auto i3_1 = imap.at("i3_1");
	auto i4_1 = imap.at("i4_1");
	auto i4_2 = imap.at("i4_2");
	
	for(size_t ii1 = 0; ii1 != i1->max(); ++ii1){
	    for(size_t ii2_1 = 0; ii2_1 != i2_1->max(); ++ii2_1){
		size_t ii2_2 = 0;
		for(size_t ii3_1 = 0; ii3_1 != i3_1->max(); ++ii3_1){
		    for(size_t ii4_1 = 0; ii4_1 != i4_1->max(); ++ii4_1){
			for(size_t ii4_2 = 0; ii4_2 != i4_2->max(); ++ii4_2){
			    const size_t jr = (((ii1*i2_1->max() + ii2_1)*i3_1->max() + ii3_1)*i4_1->max() + ii4_1)*i4_2->max() + ii4_2;
			    const size_t j1 = (((ii1*i2_1->max() + ii2_1)*i2_2->max() + ii2_2)*i3_1->max() + ii3_1)*i4_1->max() + ii4_1;
			    const size_t j2 = ((ii1*i3_1->max() + ii3_1)*i3_1->max() + ii3_1)*i4_2->max() + ii4_2;
			    auto resv = xround(res1.vdata()[jr]);
			    auto resx1v = xround(resx1.vdata()[jr]);
			    auto resx2v = xround(resx2.vdata()[jr]);
			    auto resx3v = xround(resx3.vdata()[jr]);
			    auto resx4v = xround(resx4.vdata()[jr]);
	 		    auto x12 = xround(ma1.vdata()[j1]*ma2.vdata()[j2]);
			    EXPECT_EQ( resv, x12 );
			    EXPECT_EQ( resx1v, x12 );
			    EXPECT_EQ( resx2v, x12 );
			    EXPECT_EQ( resx3v, x12 );
			    EXPECT_EQ( resx4v, x12 );
			}
		    }
		}
	    }
	    //std::cout << std::endl;
	}
    }

    TEST_F(OpTest_Dyn, Contract)
    {
	auto i1 = getIndex(cr1);
	auto di1 = getIndex(dr1);
	auto di3 = getIndex(dr3);
	auto di5 = getIndex(dr5);
	auto di6 = getIndex(dr6);
	auto di6a = getIndex(dr6a);

	(*di1)({imap["i2_1"],imap["i2_1"],imap["i3_1"],imap["i4_1"]});
	(*di3)({imap["i2_1"],imap["i5_1"]});
	(*di5)({imap["i5_1"]});
	(*di6)({imap["i3_1"],imap["i4_1"]});
	(*di6a)({imap["i3_1"],imap["i2_1"],imap["i5_1"]});

	auto resx1 = res2;
	auto resx2 = res2;
	auto resx3 = res2;
	auto resx4 = res2;
        res2(i1,di6) += (ma1(i1,di1) * ma5(di5)).c(di3);
	resx1(i1,di6) += (mkDynOp(ma1(i1,di1)) * mkDynOp(ma5(di5))).c(di3);
        resx2(i1,di6) += mkDynOp((ma1(i1,di1) * ma5(di5)).c(di3));
	resx3(i1,di6) += mkDynOp((mkDynOp(ma1(i1,di1)) * mkDynOp(ma5(di5))).c(di3));

	auto xx = std::make_shared<decltype(resx4)>(resx4);
	auto mi = mkMIndex(i1,di6a);

	auto op1 = ma1(i1,di1);
	auto op2 = ma5(di5);
	auto opr = resx4(i1,di6);

	
	auto loop = mkPILoop
	    ( [&opr,&op1,&op2,&xx,&di3,this](){
		auto dop1 = mkDynOutOp(op1 * op2, ci4_1);
		return mkGetExpr
		    (dop1,mkILoop
		     (std::make_tuple(opr,*dop1.data()->mOp), std::make_tuple(ci4_1),
		      std::make_tuple(xx),
		      std::make_tuple(opr.plus( *dop1.data()->mOp, mkMIndex(ci4_1) )),
		      std::array<size_t,1>({1}), std::array<size_t,1>({0}))); } );

        mi->pifor(1,loop)();
	
	auto i2_1 = imap.at("i2_1");
	auto i3_1 = imap.at("i3_1");
	auto i4_1 = imap.at("i4_1");
	auto i5_1 = imap.at("i5_1");
	
	for(size_t ii1 = 0; ii1 != i1->max(); ++ii1){
            for(size_t ii3_1 = 0; ii3_1 != i3_1->max(); ++ii3_1){
                for(size_t ii4_1 = 0; ii4_1 != i4_1->max(); ++ii4_1){
                    double vv = 0;
                    const size_t jr = (ii1*i3_1->max() + ii3_1)*i4_1->max() + ii4_1;
                    for(size_t ii2_1 = 0; ii2_1 != i2_1->max(); ++ii2_1){
                        const size_t j1 = (((ii1*i2_1->max() + ii2_1)*i2_1->max() + ii2_1)*i3_1->max() + ii3_1)*i4_1->max() + ii4_1;
                        for(size_t ii5_1 = 0; ii5_1 != i5_1->max(); ++ii5_1){
                            const size_t j2 = ii5_1;
                            vv += ma1.vdata()[j1] * ma5.vdata()[j2];
                        }
                    }
		    
                    auto resv = xround(res2.vdata()[jr]);
                    auto resx1v = xround(resx1.vdata()[jr]);
                    auto resx2v = xround(resx2.vdata()[jr]);
                    auto resx3v = xround(resx3.vdata()[jr]);
                    auto resx4v = xround(resx4.vdata()[jr]);
                    auto x12 = xround(vv);
                    EXPECT_EQ( resv, x12 );
                    EXPECT_EQ( resx1v, x12 );
                    EXPECT_EQ( resx2v, x12 );
                    EXPECT_EQ( resx3v, x12 );
                    EXPECT_EQ( resx4v, x12 );
		}
	    }
	    //std::cout << std::endl;
	}

    }

    typedef vector<std::string> svec;
    
    TEST_F(OpTest_Dyn, Functional)
    {
	auto i1 = getIndex(cr1);
	auto di1 = getIndex(dr1);
	auto di2 = getIndex(dr2);
	auto di4 = getIndex(dr4);
	auto di4a = getIndex(dr4a);
	
	(*di1)({"ia_1","ia_2","ib_1","ic_1"});
	(*di2)({"ib_1","ib_1","ic_2"});
	(*di4)({"ia_1","ib_1","ic_1","ic_2"});
	(*di4a)(svec({"ia_1","ib_1"}));
	auto ic_1 = DynamicIndex::getIndexFromMap<CI>("ic_1");
	auto ic_2 = DynamicIndex::getIndexFromMap<CI>("ic_2");
	
	auto resx1 = res1;
	auto resx2 = res1;
	auto resx3 = res1;
	auto resx4 = res1;
	res1(i1,di4) = ma1(i1,di1) * exp(ma2(i1,di2));
	resx1(i1,di4) = mkDynOp(ma1(i1,di1)) * exp(mkDynOp(ma2(i1,di2)));
        resx2(i1,di4) = mkDynOp(ma1(i1,di1) * exp(ma2(i1,di2)));
	resx3(i1,di4) = mkDynOp(mkDynOp(ma1(i1,di1)) * mkDynOp(exp(mkDynOp(ma2(i1,di2)))));
	
	auto xx = mkArrayPtr<double>(nullr());
	auto mi = mkMIndex(i1,di4a);

	auto hop1 = mkHLO(ma2(i1,di2));
	auto hop3 = mkHLO(ma1(i1,di1));
	auto hop2 = hl_exp(hop1);
	auto hop4 = hop3 * hop2;
	auto hopr = mkHLO(resx4(i1,di4));
	hopr.assign( hop4, mi, ic_1, ic_2 );
	
	auto i2_1 = imap.at("i2_1");
	auto i2_2 = imap.at("i2_2");
	auto i3_1 = imap.at("i3_1");
	auto i4_1 = imap.at("i4_1");
	auto i4_2 = imap.at("i4_2");
	
	for(size_t ii1 = 0; ii1 != i1->max(); ++ii1){
	    for(size_t ii2_1 = 0; ii2_1 != i2_1->max(); ++ii2_1){
		size_t ii2_2 = 0;
		for(size_t ii3_1 = 0; ii3_1 != i3_1->max(); ++ii3_1){
		    for(size_t ii4_1 = 0; ii4_1 != i4_1->max(); ++ii4_1){
			for(size_t ii4_2 = 0; ii4_2 != i4_2->max(); ++ii4_2){
			    const size_t jr = (((ii1*i2_1->max() + ii2_1)*i3_1->max() + ii3_1)*i4_1->max() + ii4_1)*i4_2->max() + ii4_2;
			    const size_t j1 = (((ii1*i2_1->max() + ii2_1)*i2_2->max() + ii2_2)*i3_1->max() + ii3_1)*i4_1->max() + ii4_1;
			    const size_t j2 = ((ii1*i3_1->max() + ii3_1)*i3_1->max() + ii3_1)*i4_2->max() + ii4_2;
			    auto resv = xround(res1.vdata()[jr]);
			    auto resx1v = xround(resx1.vdata()[jr]);
			    auto resx2v = xround(resx2.vdata()[jr]);
			    auto resx3v = xround(resx3.vdata()[jr]);
			    auto resx4v = xround(resx4.vdata()[jr]);
	 		    auto x12 = xround(ma1.vdata()[j1]*exp(ma2.vdata()[j2]));
			    EXPECT_EQ( resv, x12 );
			    EXPECT_EQ( resx1v, x12 );
			    EXPECT_EQ( resx2v, x12 );
			    EXPECT_EQ( resx3v, x12 );
			    EXPECT_EQ( resx4v, x12 );
			}
		    }
		}
	    }
	    //std::cout << std::endl;
	}
    }
    
}
