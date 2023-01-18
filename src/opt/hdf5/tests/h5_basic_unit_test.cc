
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
	    mGrps = { "gr1", "gr2" };
	}

	String mFileName;
	Vector<String> mGrps;
    };

    TEST_F(NoFile_Test, NoFile)
    {
	File f(mNoFileName, true);
	EXPECT_THROW(f.open(), std::runtime_error);
    }

    TEST_F(NoFile_Test, NoH5Format)
    {
	File f1(mWrongFileName, true);
	File f2(mWrongFileName, false);
	EXPECT_THROW(f1.open(), std::runtime_error);
	EXPECT_THROW(f2.open(), std::runtime_error);
    }

    TEST_F(Group_Test, Create)
    {
	std::remove(mFileName.c_str());
	File h5f(mFileName, false);
	EXPECT_FALSE(h5f.ro());
	h5f.open();
	h5f.addGroup("gr1");
	h5f.addGroup("gr2");
	EXPECT_EQ(h5f.get().size(), 2u);
    }

    TEST_F(Group_Test, Read)
    {
	File h5f(mFileName, true);
	h5f.open();
	EXPECT_TRUE(h5f.ro());
	EXPECT_EQ(h5f.get().size(), 2u);
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
