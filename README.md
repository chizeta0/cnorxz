# Multi Array Container Library

## Description

This library provides a framework for handling multi dimensional containers. This includes the basic container class template `MultiArrayBase` and their derivates, the Range class templates on which the containers are defined, as well as some operation class templates, which allow comfortable calling of operations on the containers.

## Build instructions

The library can be installed by the following procedure (`gtest` required):

```bash
git clone git@rqcd.ur.de:czimmermann/multi_array.git <SOURCE_DIR>
cd <BUILD_DIR>
cmake -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> <SOURCE_DIR>
make install
```

## Usage and Examples

To use the features of the libraries one has to include `multi_array_header.h` and link against the `libmultiarray.so`.
The tools of the library are accessible within the namespace `MultiArrayTools`.

### Some Terms

* Ranges: A set of meta data which is mapped onto the linear position. Every container is supposed to be defined on at least one Range. In the simplest case a Range space "containes" the numbers [0,...,rangesize]. Ranges have to be created via the corresponding factory and only exist within a shared pointer. They cannot be copied. The reason is, that every operation performed on the containers, has to identify common sets of ranges, on which the containers are defined. Every range inherits the `RangeBase` class.

* Indices: They can be thought of as iterators of the range space. In the special case of the `ContainerIndex` it is also an iterator over the corresponding container. The index type must be known at compile time (static polymorphism). Indices on a range can be obtained by `getIndex( range )`. For Container Indices call the `begin()` function of a `MultiArray`. Indices are crucial for defining operations.

* `MultiArray<T,Ranges...>`: Contains data of type `T` and is defined on `Ranges...`.
* `[Const]Slice<T,Ranges...>`: Views data of type `T` and acts as a MultiArray defined on `Ranges...`.
* `FunctionalMultiArray`: Additional template argument indicating the funtion. The value at some position is then defined by the function value obtained from the meta data of the indices/ranges.

On each of these types one can perform an operation by calling `operator(...)` where within the braces the corresponding indices have to be specified (see example below).

### Example

```c++

#include "multi_array_header.h"

namespace mat = MultiArrayTools;

typedef mat::SingleRange<double,mat::SpaceType::ANY> DRange;
typedef mat::SingleRangeFactory<double,mat::SpaceType::ANY> DRangeF;
typedef mat::SingleRange<std::string,mat::SpaceType::ANY> SRange;
typedef mat::SingleRangeFactory<std::string,mat::SpaceType::ANY> SRangeF;
typedef mat::SingleRange<size_t,mat::SpaceType::NONE> CRange; // also provided as 'ClassicR'
typedef mat::SingleRangeFactory<size_t,mat::SpaceType::NONE> CRangeF;

/*create ranges*/
std::vector<double> meta1({...});
std::vector<double> meta2({...});
std::vector<std::string> meta3({...});

DRangeF drf1(meta1);
DRangeF drf2(meta2);
SRangeF srf(meta3);
CRangeF crf(10); // specialized in this case; only size needed.

auto dr1 = mat::createExplicit( drf1 );
auto dr2 = mat::createExplicit( drf2 );
auto sr = mat::createExplicit( srf );
auto cr = mat::createExplicit( crf );

mat::MultiArray<double,DRange,DRange,SRange,CRange> ma_a(dr1,dr2,sr,cr,0);
mat::MultiArray<double,SRange,SRange> ma_b(sr,sr,0);

/* assign some values... */
double val = 1.;
for(auto& x: ma_a){
    x = val += 1;
}

for(auto& x: ma_b){
    x = val /= 2.;
}

mat::FunctionalMultiArray<double,plus<double>,DRange,DRange> fma(dr1,dr2);
mat::MultiArray<double,DRange,DRange> res1(dr1,dr2);
mat::MultiArray<double,DRange,DRange> res2(dr1,dr2);

/* get indices... */
auto i1 = mat::getIndex( dr1 );
auto i2 = mat::getIndex( dr2 );
auto i3 = mat::getIndex( sr );
auto i4 = mat::getIndex( sr );
auto i5 = mat::getIndex( cr );

i3->at(<metaString>);
i5->at(<metaNum>);

/* performs plus operation on each element in hypercube spanned by i1,i2,i4
* and sums up resulting values by iterating over i4. i3 and i5 are fixed to the positions
* given by the meta data above : */
res1(i1,i2) += (ma_a(i1,i2,i4,i5) + ma_b(i4,i3)).c(i4); 

/* calculates i2.meta() + i1.meta() and divide res1 at the corresponding entry by the result */
res2(i1,i2) = res1(i1,i2) / fma(i2,i1)
```

Further examples will follow; you can also look in the test executable source files in `./src/test/*.cc`

# Multithreading

Multithreading can be enabled by the operation class member function `par()`, which will parallelize the top loop. E.g. for the operation in the example above:
```c++
/* Enable multithreading for the loop over i1 */
res1(i1,i2).par() = (ma_a(i1,i2,i4,i5) + ma_b(i4,i3)).c(i4);
```
Caution: If you want to multithread operations over functional multi arrays, you have to make sure by yourself that involved functors are either static or const (const member variables).

# Planned features

Basically the structure of the operation routines allows in some cases (at least in those, where it is possible for simple c++ for loops) vectorization. So far the necessary implamentations are not done, but it is planned to enable these features.

# Known Issues

I didn't care that much about error handling because of missing time. There it might happen that you declare a `MultiArray` or `Slice` and an operation on it yields a segfault because it was forgotten to instancate the underlying container. Of course, there should be some mechanism, that catches such things, but due to the afore mentioned lack of time, it has not been implemented in every case. If you encounter such problems, pleas tell me, but be aware, that I won't fix it very soon.

There are unit tests, which cover the all crucial parts of this library, but probably not every special case. Hence, there is no warranty, that, if you do something fancy I didn't consider, everything works fine.