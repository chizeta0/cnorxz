
macro(check_avx)
  include(CheckCXXSourceCompiles)
  set(CMAKE_REQUIRED_FLAGS "-Wall -Werror -Wpedantic -std=c++17 -mavx")
  check_cxx_source_compiles("
#include <immintrin.h>
#include <iostream>
int main()
{
const double a[4] = { 0,0,0,0 };
const double b[4] = { 0,0,0,0 };
double o[4] = { 0,0,0,0 };
__m256d av = _mm256_load_pd(a);
__m256d bv = _mm256_load_pd(b);
__m256d ov = _mm256_add_pd(av, bv);
_mm256_store_pd(o, ov);
std::cout << o[0] << std::endl;
return 0;
}
"
    AVX_AVAIL
    )
endmacro()

