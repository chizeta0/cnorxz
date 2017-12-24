// -*- C++ -*-

#include <cstdlib>
#include <iostream>
#include <cassert>

#include "multi_array_header.h"

#include <ctime>
#include <cmath>

#define ONLY_SPIN

namespace MAT = MultiArrayTools;

namespace {

    double xround(double arg)
    {
	return roundf(arg * 100000.) / 100000.;
    }
    
    using namespace MAT;

    template <class Factory>
    void swapFactory(std::shared_ptr<RangeFactoryBase>& fptr)
    {
	auto nptr = std::make_shared<Factory>();
	fptr = nptr;
    }
    
    template <class Factory, typename T>
    void swapFactory(std::shared_ptr<RangeFactoryBase>& fptr, std::initializer_list<T> ilist)
    {
	std::vector<T> tmp = ilist;
	auto nptr = std::make_shared<Factory>( tmp );
	fptr = nptr;
    }

    template <class Factory, typename T>
    void swapFactory(std::shared_ptr<RangeFactoryBase>& fptr, std::vector<T>& ilist)
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

    template <typename... Ts>
    auto mkts(Ts&&... ts) -> decltype(std::make_tuple(ts...))
    {
	return std::make_tuple(static_cast<size_t>( ts )...);
    }
#ifndef ONLY_SPIN    
    class OpTest_Performance
    {
    public:

	typedef SingleRangeFactory<size_t,SpaceType::ANY> SRF;
	typedef SRF::oType SRange;

	typedef MultiRangeFactory<SRange,SRange> MRF;
	typedef MRF::oType MRange;

	OpTest_Performance()
	{
	
	    std::vector<size_t> initvec1(vs1);
	    cv1.resize(vs1);
	    for(size_t i = 0; i != vs1; ++i){
		initvec1[i] = i;
		cv1[i] = sqrt( static_cast<double>(i)*0.53 );
	    }

	    std::vector<size_t> initvec2(vs2);
	    cv2.resize(vs2*vs1);
	    for(size_t i = 0; i != vs2; ++i){
		initvec2[i] = i;
		for(size_t j = 0; j != vs1; ++j){
		    cv2[i*vs1 + j] = static_cast<double>(i) * sin(static_cast<double>(j)*0.4);
		}
	    }

	    swapFactory<SRF>(rfbptr, initvec1);
	    sr1ptr = std::dynamic_pointer_cast<SRange>(rfbptr->create());

	    swapFactory<SRF>(rfbptr, initvec2);
	    sr2ptr = std::dynamic_pointer_cast<SRange>(rfbptr->create());

	    swapMFactory<MRF>(rfbptr, sr2ptr, sr1ptr);
	    mrptr = std::dynamic_pointer_cast<MRange>(rfbptr->create());
	}

	void PCheck();

	
    private:
	//const size_t vs1 = 10000;
	//const size_t vs2 = 1000;
	const size_t vs1 = 4000;
	const size_t vs2 = 2500;

	std::shared_ptr<RangeFactoryBase> rfbptr;
	std::shared_ptr<SRange> sr1ptr;
	std::shared_ptr<SRange> sr2ptr;
	std::shared_ptr<MRange> mrptr;

	std::vector<double> cv1;
	std::vector<double> cv2;
    };
#endif
    class OpTest_Spin
    {
    public:

	typedef SpinRF SRF;
	typedef SpinRange SR;
	typedef MultiRangeFactory<SR,SR,SR,SR,SR,SR,SR,SR> SR8F;
	typedef SR8F::oType SR8;

	static const size_t s = 65536;
	
	OpTest_Spin()
	{
	    data.resize(s);
	    for(size_t i = 0; i != s; ++i){
		double arg = static_cast<double>( i - s ) - 0.1; 
		data[i] = sin(arg)/arg;
	    }
	    SRF f;
	    sr = std::dynamic_pointer_cast<SR>(f.create());
	}

	void contract();
	
    private:
	
	std::vector<double> data;
	std::shared_ptr<SR> sr;
    };

    void OpTest_Spin::contract()
    {
	MultiArray<double,SR,SR,SR,SR,SR,SR,SR,SR> ma(sr, sr, sr, sr, sr, sr, sr, sr, data);
	MultiArray<double,SR,SR> res1( sr, sr );
	
	auto alpha = MAT::getIndex<SR>();
	auto beta = MAT::getIndex<SR>();
	auto gamma = MAT::getIndex<SR>();
	auto delta = MAT::getIndex<SR>();
	auto deltap = MAT::getIndex<SR>();
	auto vdeltap = MAT::make_viwb(deltap);
	
	auto mix = MAT::mkMIndex( alpha, beta, gamma );

	std::clock_t begin = std::clock();
	res1(delta, deltap).set(vdeltap) = ma(delta, alpha, alpha, beta, beta, gamma, gamma, deltap).c(mix);
	std::clock_t end = std::clock();
	std::cout << "MultiArray time: " << static_cast<double>( end - begin ) / CLOCKS_PER_SEC
		  << std::endl;
	
	std::vector<double> vres(4*4);

	std::clock_t begin2 = std::clock();
	for(size_t d = 0; d != 4; ++d){
	    for(size_t p = 0; p != 4; ++p){
		const size_t tidx = d*4 + p;
		vres[tidx] = 0.;
		for(size_t a = 0; a != 4; ++a){
		    for(size_t b = 0; b != 4; ++b){
			for(size_t c = 0; c != 4; ++c){
			    const size_t sidx = d*4*4*4*4*4*4*4 + a*5*4*4*4*4 + b*5*4*4*4 + c*5*4 + p;
			    vres[tidx] += data[sidx];
			}
		    }
		}
	    }
	}
	std::clock_t end2 = std::clock();

	assert( xround(res1.at(mkts(0,0))) == xround(vres[0]) );
	assert( xround(res1.at(mkts(0,1))) == xround(vres[1]) );
	assert( xround(res1.at(mkts(0,2))) == xround(vres[2]) );
	assert( xround(res1.at(mkts(0,3))) == xround(vres[3]) );
	
	assert( xround(res1.at(mkts(1,0))) == xround(vres[4]) );
	assert( xround(res1.at(mkts(1,1))) == xround(vres[5]) );
	assert( xround(res1.at(mkts(1,2))) == xround(vres[6]) );
	assert( xround(res1.at(mkts(1,3))) == xround(vres[7]) );

	assert( xround(res1.at(mkts(2,0))) == xround(vres[8]) );
	assert( xround(res1.at(mkts(2,1))) == xround(vres[9]) );
	assert( xround(res1.at(mkts(2,2))) == xround(vres[10]) );
	assert( xround(res1.at(mkts(2,3))) == xround(vres[11]) );

	assert( xround(res1.at(mkts(3,0))) == xround(vres[12]) );
	assert( xround(res1.at(mkts(3,1))) == xround(vres[13]) );
	assert( xround(res1.at(mkts(3,2))) == xround(vres[14]) );
	assert( xround(res1.at(mkts(3,3))) == xround(vres[15]) );

	std::cout << "std::vector - for loop time: " << static_cast<double>( end2 - begin2 ) / CLOCKS_PER_SEC
		  << std::endl;
	std::cout << "ratio: " << static_cast<double>( end - begin ) / static_cast<double>( end2 - begin2 ) << std::endl;
    }    
#ifndef ONLY_SPIN    
    void OpTest_Performance::PCheck()
    {
	MultiArray<double,MRange> ma2(mrptr, cv2);
	MultiArray<double,SRange> ma1(sr1ptr, cv1);
	MultiArray<double,MRange> res(mrptr);

	auto si1 = MAT::getIndex(sr1ptr);
	auto si2 = MAT::getIndex(sr2ptr);
	auto mi = MAT::getIndex(mrptr);
	(*mi)(si2, si1);

	std::clock_t begin = std::clock();
	
	res(mi) = ma2(mi) * ma1(si1);

	std::clock_t end = std::clock();
	std::cout << "MultiArray time: " << static_cast<double>( end - begin ) / CLOCKS_PER_SEC
		  << std::endl;

	std::vector<double> res2(vs1*vs2);
	std::clock_t begin2 = std::clock();
	
	for(size_t i = 0; i != vs2; ++i){
	    for(size_t j = 0; j != vs1; ++j){
		res2[i*vs1 + j] = cv1[j] * cv2[i*vs1 + j];
	    }
	}

	std::clock_t end2 = std::clock();
	std::cout << "std::vector - for loop time: " << static_cast<double>( end2 - begin2 ) / CLOCKS_PER_SEC
		  << std::endl;

	std::cout << "ratio: " << static_cast<double>( end - begin ) / static_cast<double>( end2 - begin2 ) << std::endl;

	assert( xround( res.at(mkt(7,9)) ) == xround(res2[7*vs1 + 9]) );
	//assert( xround( res.at(mkt(700,900)) ) == xround(res2[700*vs1 + 900]) );
	
    }
#endif    
} // anonymous namspace

int main(int argc, char** argv)
{
#ifndef ONLY_SPIN
    OpTest_Performance pt;
    pt.PCheck();
#endif
    OpTest_Spin st;
    st.contract();

    
    return 0;
}
