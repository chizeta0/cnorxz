
#include "test_header.h"

namespace
{
    class OpTest_Spin : public ::testing::Test
    {
    protected:

	typedef SpinRF SRF;
	typedef SpinRange SR;
	typedef MultiRangeFactory<SR,SR,SR,SR,SR,SR,SR,SR> SR8F;
	typedef SR8F::oType SR8;

	static const size_t s = 65536;
	
	OpTest_Spin()
	{
	    data.resize(s);
	    for(size_t i = 0; i != s; ++i){
		const double arg = static_cast<double>( s - i ) - 0.1; 
		data[i] = sin(arg)*10;
	    }
	    SRF f;
	    sr = std::dynamic_pointer_cast<SR>(f.create());
	}

	vector<double> data;
	std::shared_ptr<SR> sr;
    };

    TEST_F(OpTest_Spin, Contract)
    {
	Array<double,SR,SR,SR,SR,SR,SR,SR,SR> ma(sr, sr, sr, sr, sr, sr, sr, sr, data);
	Array<double,SR,SR> res1( sr, sr );
	Array<double,SR,SR> res2( sr, sr );
	
	auto alpha = MAT::getIndex<SR>();
	auto beta = MAT::getIndex<SR>();
	auto gamma = MAT::getIndex<SR>();
	auto delta = MAT::getIndex<SR>();
	auto deltap = MAT::getIndex<SR>();
	
	auto mix = MAT::mkMIndex( alpha, beta, gamma );

	std::clock_t begin = std::clock();
	res1(delta, deltap) += ma(delta, alpha, alpha, beta, beta, gamma, gamma, deltap).c(mix);
	//res1(delta, deltap) += ma(delta, alpha, alpha, beta, beta, gamma, gamma, deltap);
	std::clock_t end = std::clock();
	std::cout << "Array time: " << static_cast<double>( end - begin ) / CLOCKS_PER_SEC
		  << std::endl;

	res2(delta, deltap).par() += ma(delta, alpha, alpha, beta, beta, gamma, gamma, deltap).c(alpha).c(beta).c(gamma);
	
	vector<double> vres(4*4);

	std::clock_t begin2 = std::clock();
	for(size_t d = 0; d != 4; ++d){
	    for(size_t p = 0; p != 4; ++p){
		const size_t tidx = d*4 + p;
		vres[tidx] = 0.;
		for(size_t a = 0; a != 4; ++a){
		    for(size_t b = 0; b != 4; ++b){
			for(size_t c = 0; c != 4; ++c){
			    const size_t sidx = d*4*4*4*4*4*4*4 + a*5*4*4*4*4*4 + b*5*4*4*4 + c*5*4 + p;
			    vres[tidx] += data[sidx];
			}
		    }
		}
	    }
	}
	std::clock_t end2 = std::clock();

	EXPECT_EQ( xround(res1.at(mkts(0,0))), xround(vres[0]) );
	EXPECT_EQ( xround(res1.at(mkts(0,1))), xround(vres[1]) );
	EXPECT_EQ( xround(res1.at(mkts(0,2))), xround(vres[2]) );
	EXPECT_EQ( xround(res1.at(mkts(0,3))), xround(vres[3]) );
	
	EXPECT_EQ( xround(res1.at(mkts(1,0))), xround(vres[4]) );
	EXPECT_EQ( xround(res1.at(mkts(1,1))), xround(vres[5]) );
	EXPECT_EQ( xround(res1.at(mkts(1,2))), xround(vres[6]) );
	EXPECT_EQ( xround(res1.at(mkts(1,3))), xround(vres[7]) );

	EXPECT_EQ( xround(res1.at(mkts(2,0))), xround(vres[8]) );
	EXPECT_EQ( xround(res1.at(mkts(2,1))), xround(vres[9]) );
	EXPECT_EQ( xround(res1.at(mkts(2,2))), xround(vres[10]) );
	EXPECT_EQ( xround(res1.at(mkts(2,3))), xround(vres[11]) );

	EXPECT_EQ( xround(res1.at(mkts(3,0))), xround(vres[12]) );
	EXPECT_EQ( xround(res1.at(mkts(3,1))), xround(vres[13]) );
	EXPECT_EQ( xround(res1.at(mkts(3,2))), xround(vres[14]) );
	EXPECT_EQ( xround(res1.at(mkts(3,3))), xround(vres[15]) );

	EXPECT_EQ( xround(res2.at(mkts(0,0))), xround(vres[0]) );
	EXPECT_EQ( xround(res2.at(mkts(0,1))), xround(vres[1]) );
	EXPECT_EQ( xround(res2.at(mkts(0,2))), xround(vres[2]) );
	EXPECT_EQ( xround(res2.at(mkts(0,3))), xround(vres[3]) );
	
	EXPECT_EQ( xround(res2.at(mkts(1,0))), xround(vres[4]) );
	EXPECT_EQ( xround(res2.at(mkts(1,1))), xround(vres[5]) );
	EXPECT_EQ( xround(res2.at(mkts(1,2))), xround(vres[6]) );
	EXPECT_EQ( xround(res2.at(mkts(1,3))), xround(vres[7]) );

	EXPECT_EQ( xround(res2.at(mkts(2,0))), xround(vres[8]) );
	EXPECT_EQ( xround(res2.at(mkts(2,1))), xround(vres[9]) );
	EXPECT_EQ( xround(res2.at(mkts(2,2))), xround(vres[10]) );
	EXPECT_EQ( xround(res2.at(mkts(2,3))), xround(vres[11]) );

	EXPECT_EQ( xround(res2.at(mkts(3,0))), xround(vres[12]) );
	EXPECT_EQ( xround(res2.at(mkts(3,1))), xround(vres[13]) );
	EXPECT_EQ( xround(res2.at(mkts(3,2))), xround(vres[14]) );
	EXPECT_EQ( xround(res2.at(mkts(3,3))), xround(vres[15]) );

	std::cout << "vector - for loop time: " << static_cast<double>( end2 - begin2 ) / CLOCKS_PER_SEC
		  << std::endl;
	std::cout << "ratio: " << static_cast<double>( end - begin ) / static_cast<double>( end2 - begin2 ) << std::endl;
    }    

}
