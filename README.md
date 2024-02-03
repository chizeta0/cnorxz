# Container with Native Operation Routines and Expressions (CNORXZ)

(as always, the name was there before the acronym)

![Image](./cnorxz_logo.png)

## Description

This library provides a framework for handling multi dimensional containers, their meta data, and several kinds of operations on one or more of them.

## Build instructions

The library can be installed by the following procedure (`gtest` required):

```bash
git clone git@git.f3l.de:chizeta/cnorxz.git <LIBRARY_ROOT_DIR>
mkdir <BUILD_DIR>
cd <BUILD_DIR>
cmake -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> <LIBRARY_ROOT_DIR>
make install
```

To build the doxygen:

```bash
cd <SOURCE_DIR>/doc/doxy
doxygen Doxyfile
```

## Linking

To use the features of the libraries one has to include `cnorxz.h` and link against the `libcnorxz.so`.
The tools of the library are accessible within the namespace `CNORXZ`.

## Documentation

(Also consider doxygen)

### Basics and Library organization

This library consists of several building blocks. For simple usage, the most important building blocks are [ranges](#sec-ranges), [indices](#sec-indices) and [arrays](#sec-array-types).

#### Ranges {#sec-ranges}

Basically, a *range* defines a meta data space. There are several range class types, which are derived from the abstract base class `RangeBase`. Ranges can only be created by the corresponding factory and exclusively exist within a shared pointer; they cannot be copied. Available range class types are:

* `CRange` : Classic one-dimensional range. The meta data space is simply given by integer numbers running from `0` to `size-1`. The range size is determined at runtime.

* `URange<MetaT>` : Generic One-dimensional range. The meta data space is user defined, the meta data type is passed as template argument. The range size is determined at runtime.

* `SRange<MetaT,S>` : The same as `URange`, but the range length is fixed at compile time by the template integer variable `S`.

* `PRange<RangeT>` : Partial or sub-range, i.e. a user-defined subspace of another range. The type of the range must be known at compile time, the subspace can be specified at runtime.

* `MRange<RangeTs...>` : Multi-dimensional range, spanned by a set of ranges. The number of ranges, as well as their types must be known at compile time.

* `YRange` : The same as `MRange` but the number of ranges and their types can be specified at runtime.

#### Indices {#sec-indices}

For each range type there is a corresponding index type (`CIndex`, `UIndex<MetaT>`, `SIndex<MetaT,S>`, `PIndex<IndexT>`, `MIndex<IndexTs...>`, `YIndex`). They act as const iterators on the ranges and are a crucial component to define operations on containers. In contrast to the ranges, all index types must be known at compile time (static polymorphism, `IndexInterface<Index,MetaT>`). 

Apart from range specific indices, there exist also special indices:

* `DIndex` : Dynamic index wrapper, for the case that the index type cannot be determined at compile time.

* `AIndex<T>` : Array index. Const iterators pointing to the data of an array-type object with data type `T`.

* `BIndex<T>` : The same as `AIndex`, but not const.

#### Array types {#sec-array-types}

Finally, there are the container classes (arrays), which are derived from `CArrayBase<T>` (const) or `ArrayBase<T>` for a given data type `T`. All arrays are defined on a range, their data can be accessed or iterated over using suitable indices. The array-type actually containing data is called `MArray<T>`. Moreover, there exist array-types that do not contain data, but view the data of other arrays or at least parts of the data. These are called `CSlice<T>` (const view) or `Slice`.

#### Expressions and Operations

In the context of this library, *expressions* are classes representing an expression that is supposed to be executed at given points during an iteration proceedure that involves one or more indexed quantities. Each expression type must fulfill the following requirements:

* There must be an implementation of `operator()` taking a multi-position argument indicating the position for the current iteration of each involved object according to its indices. The function can have an expression defined return value, which can be further processed.

* There must be an implementation of `rootSteps()` taking an index ID as argument. The function should return jump sizes for each involved object corresponding to the given index.

Each expression `Xpr` should be derived from the class `XprInterface<Xpr>`, where `Xpr` is a recurring template argument (static polymorphism).

There are two important expression types:

* *For* expressions: They represent a for loop over a given index.

* *Operations*: They correspond to one or more indexed array types and given operation on or between them. If an operation handles nothing but the access to one single array, it is called *Operation* *Root*. 

Example:
```cpp
#include "cnroxz.h"

using namespace CNORZX;

RangePtr r = CRangeFactory(3).create(); // [0,1,2]
RangePtr s = CRangeFactory(5).create(); // [0,1,2,3,4]
RangePtr t = URangeFactory<Int>(Vector<Int>{4,5,6}).create(); // [4,5,6]
i = std::make_shared<CIndex>(r);
j = std::make_shared<CIndex>(s);
k = std::make_shared<UIndex<Int>>(t);

MArray<Double> a(r*s*t); // 3-dim array, dimensions = [3,5,3], size = 45
MArray<Double> b(r*t); // 2-dim array, dimensions = [3,3], size = 9
MArray<Double> c(s*t); // 2-dim array, dimensions = [3,5], size = 15

// set array element values of a,b,c here...

c(j*k) += ( a(i*j*k) * b(j*k) * xpr(k) ).c(i);

/* Explanation of the above line:

c(j*k), a(i*j*k), b(j*k) and xpr(k) are operation roots, i.e.
they manage access to the arrays c,a,b and the meta data space of index k

The operations between the operation roots are also operations in the
sense of this nomenclature, i.e. a(i*j*k) * b(j*k) is again an operation.

The operation member function c() creates contraction, i.e. a for loop
over the given index (i). If no further functions are specified, the values
returned by the underlying operations are summed up for equivalent index
combinations.

The += invokes a for loop over the indices on the l.h.s.. The values returned
by the expression on the r.h.s. are added to the l.h.s. Something similar could be done
with a = operator. Then the values would just be assigned instead of added.

*/

// Equivalent C-stype for loop (if a,b,c were C-style arrays):

for(size_t j = 0; j < s; ++j){
	for(size_t k = 0; k < t; ++k){
		int kv = k+4;
		double x = 0;
		// the contraction part:
		for(size_t i = 0; i < r; ++i){
			 x += a[i*s*t+j*t+k] * b[j*t+k] * kv;
		}
		c[j*t+k] += x;
	}
}

```

...

