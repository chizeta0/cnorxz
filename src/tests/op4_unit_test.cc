
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

	MultiArray<double,CR,DR> res1;

	std::map<std::string,std::shared_ptr<IndexW>> imap;

	std::shared_ptr<DR> dr1;
	std::shared_ptr<DR> dr2;
	std::shared_ptr<DR> dr3;
	std::shared_ptr<DR> dr4;
	std::shared_ptr<CR> cr1;
	
	OpTest_Dyn()
	{
	    cr1 = createRangeE<CR>(5);
	    
	    auto cr2 = createRangeE<CR>(7);
	    auto cr3 = createRangeE<CR>(11);
	    auto cr4 = createRangeE<CR>(3);
	    auto cr5 = createRangeE<CR>(13);

	    dr1 = createRangeE<DR>(cr2,cr2,cr3,cr4);
	    dr2 = createRangeE<DR>(cr3,cr3,cr4);
	    dr3 = createRangeE<DR>(cr2,cr5);

	    dr4 = createRangeE<DR>(cr2,cr3,cr4,cr4);

	    ma1 = mkArray<double>(cr1,dr1);
	    ma2 = mkArray<double>(cr1,dr2);
	    ma3 = mkArray<double>(dr3);

	    res1 = mkArray<double>(cr1,dr4);

	    setMARandom(ma1, 25);
	    setMARandom(ma2, 31);
	    setMARandom(ma3, 47);

	    imap["i2_1"] = mkIndexW(getIndex(cr2));
	    imap["i2_2"] = mkIndexW(getIndex(cr2));
	    imap["i3_1"] = mkIndexW(getIndex(cr3));
	    imap["i3_2"] = mkIndexW(getIndex(cr3));
	    imap["i4_1"] = mkIndexW(getIndex(cr4));
	    imap["i4_2"] = mkIndexW(getIndex(cr4));
	    imap["i5_1"] = mkIndexW(getIndex(cr5));
	    imap["i5_2"] = mkIndexW(getIndex(cr5));
	}
    };

    TEST_F(OpTest_Dyn, Multiply)
    {
	auto i1 = getIndex(cr1);
	auto di1 = getIndex(dr1);
	auto di2 = getIndex(dr2);
	auto di4 = getIndex(dr4);

	(*di1)({imap["i2_1"],imap["i2_2"],imap["i3_1"],imap["i4_1"]});
	(*di2)({imap["i3_1"],imap["i3_1"],imap["i4_2"]});
	(*di4)({imap["i2_1"],imap["i3_1"],imap["i4_1"],imap["i4_2"]});

	auto resx1 = res1;
	res1(i1,di4) = ma1(i1,di1) * ma2(i1,di2);

	resx1(i1,di4) = mkDynOp(ma1(i1,di1)) * mkDynOp(ma2(i1,di2));
	
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
			    auto resx = xround(res1.vdata()[jr]);
			    //std::cout << resx << "  ";
			    auto x12 = xround(ma1.vdata()[j1]*ma2.vdata()[j2]);
			    EXPECT_EQ( resx, x12 );
			}
		    }
		}
	    }
	    //std::cout << std::endl;
	}
    }

    TEST_F(OpTest_Dyn, Contract)
    {

    }

    TEST_F(OpTest_Dyn, Functional)
    {

    }
    
}
