
#include <cstdlib>
#include <iostream>

#include "ranges/ranges.h"
#include "operation/operation.h"
#include "array/array.h"

#include <chrono>
#include <random>

namespace
{
    using namespace CNORXZ;
}

int main()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-9.9, 10.1);
    
    auto r0 = CRangeFactory(4).create();
    auto r1 = CRangeFactory(8).create();
    auto r2 = CRangeFactory(890).create();
    auto r3 = CRangeFactory(970).create();

    auto i = std::make_shared<SIndex<SizeT,4>>(r0);
    auto vi = eindexPtr<1>( std::make_shared<SIndex<SizeT,4>>(r0) );
    //auto vi = lindexPtr<1>( std::make_shared<SIndex<SizeT,4>>(r0) );
    auto i1i = lindexPtr<2>( std::make_shared<SIndex<SizeT,8>>(r1) );
    auto i1j = lindexPtr<3>( std::make_shared<SIndex<SizeT,8>>(r1) );
    auto i1k = lindexPtr<4>( std::make_shared<SIndex<SizeT,8>>(r1) );
    auto i2i = std::make_shared<CIndex>(r2);
    auto i3i = std::make_shared<CIndex>(r3);
    
    MArray<Double> a(r2*r1*r1*r0);
    a(i2i*i1i*i1j*i) = operation( [&](auto x) { return x+dist(gen); }, posop(i) );
    MArray<Double> b(r3*r1*r1*r0);
    b(i3i*i1j*i1k*i) = operation( [&](auto x) { return x+dist(gen); }, posop(i) );
    MArray<Double> c(r1*r1*r0);

    //const SizeT osize = mindex(i2i*i3i).lmax().val();
    const SizeT lsize = mindex(i1i*i1k*i*i2i*i3i*i1j).lmax().val();
    const SizeT nflops = lsize*2;
    std::cout << " loop size = " << lsize << " => " << nflops/1e9 << " Gflops" << std::endl;
    std::cout << " -> start operation" << std::endl;
    //auto thexpr = mindexPtr(i1i*i1k*i2i*i3i*i1j*vi)->ifor( operation( [&](auto& t, const auto s) { t += s; }, c(i1i*i1k*vi), a(i2i*i1i*i1j*vi)*b(i3i*i1j*i1k*vi) ), NoF{} );
    auto thexpr = mindexPtr(i2i*i3i*i1i*i1j*i1k*vi)->ifor( operation( [&](auto& t, const auto s) { t += s; }, c(i1i*i1k*vi), a(i2i*i1i*i1j*vi)*b(i3i*i1j*i1k*vi) ), NoF{} );
    auto tbeg = std::chrono::system_clock::now();
    //c(i1i*i1k*vi) += (a(i2i*i1i*i1j*vi)*b(i3i*i1j*i1k*vi)).c(mindexPtr(i2i*i3i*i1j));
    //for(SizeT cnt = 0; cnt != )
    thexpr();
    auto tend = std::chrono::system_clock::now();
    Double dt = static_cast<Double>( std::chrono::duration<Double>(tend-tbeg).count() );
    std::cout << " -> done: " << dt << " secs" << std::endl;
    std::cout << " performance: " << nflops/1e9/dt << " Gflops/sec " << std::endl;
    
    Double s = 0;
    for(const auto& x: c){
	s += x;
    }
    std::cout << " res = " << s << std::endl;
    
    return 0;
}
