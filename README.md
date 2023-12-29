# Container with Native Operation Routines by XZ (CNORXZ)

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

This library consists of several building blocks. For simple usage, the most important building blocks are [ranges](#ranges), [indices](#indices) and [array types](#arrays).

#### Ranges

Basically, a *range* defines a meta data space. There are several range class types, which are derived from the abstract base class `RangeBase`. Ranges can only be created by the corresponding factory and exclusively exist within a shared pointer; they cannot be copied. Available range class types are:

* `CRange` : Classic one-dimensional range. The meta data space is simply given by integer numbers running from `0` to `size-1`. The range size is determined at runtime.

* `URange<MetaT>` : Generic One-dimensional range. The meta data space is user defined, the meta data type is passed as template argument. The range size is determined at runtime.

* `SRange<MetaT,S>` : The same as `URange`, but the range length is fixed at compile time by the template integer variable `S`.

* `PRange<RangeT>` : Partial or sub-range, i.e. a user-defined subspace of another range. The type of the range must be known at compile time, the subspace can be specified at runtime.

* `MRange<RangeTs...>` : Multi-dimensional range, spanned by a set of ranges. The number of ranges, as well as their types must be known at compile time.

* `YRange` : The same as `MRange` but the number of ranges and their types can be specified at runtime.

#### Indices

For each range type there is a corresponding index type (`CIndex`, `UIndex<MetaT>`, `SIndex<MetaT,S>`, `PIndex<IndexT>`, `MIndex<IndexTs...>`, `YIndex`). They act as const iterators on the ranges and are a crucial component to define operations on containers. In contrast to the ranges, all index types must be known at compile time (static polymorphism, `IndexInterface<Index,MetaT>`). 

Apart from range specific indices, there exist also special indices:

* `DIndex` : Dynamic index wrapper, for the case that the index type cannot be determined at compile time.

* `AIndex<T>` : Array index. Const iterators pointing to the data of an array-type object with data type `T`.

* `BIndex<T>` : The same as `AIndex`, but not const.

#### Array types

Finally, there are the container classes (arrays), which are derived from `CArrayBase<T>` (const) or `ArrayBase<T>` for a given data type `T`. All arrays are defined on a range, their data can be accessed or iterated over using suitable indices. The array-type actually containing data is called `MArray<T>`. Moreover, there exist array-types that do not contain data, but view the data of other arrays or at least parts of the data. These are called `CSlice<T>` (const view) or `Slice`.

#### Expressions and Operations

...

