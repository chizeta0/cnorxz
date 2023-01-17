
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstdio>

#include "gtest/gtest.h"

#include "cnorxz_hdf5.h"

namespace
{
    using namespace CNORXZ;
    using namespace CNORXZ::hdf5;
    
    class NoFile_Test : public ::testing::Test
    {
    protected:
	
	NoFile_Test()
	{
	    mNoFileName = "no_file.h5";
	    mWrongFileName = "just_txt.h5";
	    std::fstream fs(mWrongFileName, std::ios::out);
	    fs << "This file is not a hdf5 file" << std::endl;
	    fs.close();
	}

	String mNoFileName;
	String mWrongFileName;
    };

    class Group_Test : public ::testing::Test
    {
    protected:

	Group_Test()
	{
	    mFileName = "test_file.h5";
	    std::remove(mFileName.c_str());
	    mGrps = { "gr1", "gr2" };
	}

	String mFileName;
	Vector<String> mGrps;
    };

    TEST_F(NoFile_Test, NoFile)
    {
	auto l = [&](){ return std::make_shared<File>(mNoFileName, true); };
	EXPECT_THROW(l(), std::runtime_error);
    }

    TEST_F(NoFile_Test, NoH5Format)
    {
	auto l1 = [&](){ return std::make_shared<File>(mWrongFileName, true); };
	auto l2 = [&](){ return std::make_shared<File>(mWrongFileName, false); };
	EXPECT_THROW(l1(), std::runtime_error);
	EXPECT_THROW(l2(), std::runtime_error);
    }

    TEST_F(Group_Test, Create)
    {
	File h5f(mFileName, false);
	EXPECT_FALSE(h5f.ro());
	//h5f.append("gr1");
	auto grange = URangeFactory<String>( mGrps ).create();
	h5f.set( grange );
	for(auto i = grange->begin(); i != grange->end(); ++i){
	    (*h5f.get())[i] = std::make_shared<Group>( i.meta().str(), &h5f );
	}
    }

    TEST_F(Group_Test, Read)
    {
	File h5f(mFileName, true);
	EXPECT_TRUE(h5f.ro());
	
    }
}

// check write to new file
// check read from that file
// check write to existing file
// check again read from that file

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
