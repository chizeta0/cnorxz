// -*- C++ -*-
/**
   
   @file opt/hdf5-mpi/tests/h5_mpi_basic_unit_test.cc
   @brief cnorxz hdf5 mpi basic unit tests.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstdio>

#include "gtest/gtest.h"

#include "cnorxz_hdf5.h"
#include "cnorxz_hdf5_mpi.h"
#include "test_numbers.h"
#include "mpi_env.h"

namespace
{
    using namespace CNORXZ;
    using namespace CNORXZ::hdf5;
    using namespace CNORXZ::mpi;
    using Test::Numbers;

    typedef MIndex<CIndex,CIndex,CIndex,CIndex> C4;
    
    class RDataset_test : public ::testing::Test
    {
    protected:

	RDataset_test()
	{
	    mFilename = "parallel_test_file.h5";
	    Vector<RangePtr> grv(4);
	    grv[0] = CRangeFactory(5).create();
	    grv[1] = CRangeFactory(14).create();
	    grv[2] = CRangeFactory(10).create();
	    grv[3] = CRangeFactory(3).create();
	    const RangePtr g1 = CRangeFactory(1).create();
	    const RangePtr g2 = CRangeFactory(2).create();
	    mGeom = yrange(Vector<RangePtr>{g1,g2,g2,g1});
	    const RangePtr gr = yrange(grv);
	    const RangePtr rr = rrange(gr,mGeom);
	    mLR = rr->sub(1);
	    Vector<Double> vec = Numbers::get(14,gr->size()+10);
	    Vector<Double> loc(mLR->size());
	    for(auto k = CIndex(mGeom); k.lex() != k.lmax().val(); ++k){
		if(k.lex() == getRankNumber()){
		    std::copy(vec.begin()+k.lex()*mLR->size(), vec.begin()+(k.lex()+1)*mLR->size(), loc.begin());
		}
	    }
	    auto locarr = MArray<Double>( mLR, loc );
	    mA = RArray<Double>( locarr, mGeom );
	}
	
	RArray<Double> mA;
	RangePtr mGeom;
	RangePtr mLR;
	String mFilename;
    };

    TEST_F(RDataset_test, Write)
    {
	if(getRankNumber() == 0){
	    std::remove(mFilename.c_str());
	}
	MPI_Barrier(MPI_COMM_WORLD);
	RFile h5f(mFilename, false);
	h5f.open();
	h5f.addGroup("dir");
	auto dir = h5f.getGroup("dir");
	dir->add("dat", [](const String& name, const ContentBase* par, const RArray<Double>& d)
	{ auto o = std::make_shared<SRDataset<Double>>( name, par ); o->init(d); return o; }, mA );
	h5f.close();
    }

    TEST_F(RDataset_test, Read)
    {
	RFile h5f(mFilename, false);
	h5f.open();
	//h5f.addGroup("dir");
	auto dir = h5f.getGroup("dir");
	CHECK;
	MPI_Barrier(MPI_COMM_WORLD);
	dir->open();
	CHECK;
	MPI_Barrier(MPI_COMM_WORLD);
	auto dat = dir->get("dat", [](const String& name, const ContentBase* par, auto& i)
	{ i->close(); auto dset = std::make_shared<SRDataset<Double>>(name, par); i = dset;
	    return dset; } );
	CHECK;
	MPI_Barrier(MPI_COMM_WORLD);
	auto a = dat->read(mGeom);
	CHECK;
	MPI_Barrier(MPI_COMM_WORLD);
	dat->close();
	CHECK;
	MPI_Barrier(MPI_COMM_WORLD);
	dir->close();
	CHECK;
	MPI_Barrier(MPI_COMM_WORLD);
	h5f.close();
	CHECK;
	MPI_Barrier(MPI_COMM_WORLD);

	auto i = std::make_shared<CIndex>(mLR);
	i->ifor( operation( [](Double a, Double b) { EXPECT_EQ(a,b); }, mA(i), a(i) ), NoF{} )();
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment( new MPIEnv(argc, argv) );
    return RUN_ALL_TESTS();
}
