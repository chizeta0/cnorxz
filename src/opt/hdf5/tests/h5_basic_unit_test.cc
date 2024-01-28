
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

    static const String testh5file = "test_file.h5";
    
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
	    mFileName = testh5file;
	    //mGrps = { "gr1", "gr2", "foo", "bar", "moregroups" };
	    mFs = {"field1","second","real"};
	    Vector<Tuple<SizeT,Int,Double>> v
		( { {0, -6, 3.141},
		    {3, -8, 0.789},
		    {34, 4, 10.009},
		    {2, -777, -9.77},
		    {321, 0, -0.003}
		} );
	    RangePtr rs = CRangeFactory(v.size()).create();
	    mTabA = MArray<Tuple<SizeT,Int,Double>>(rs, std::move(v)); 
	}

	String mFileName;
	//Vector<String> mGrps;

	Arr<String,3> mFs;
	MArray<Tuple<SizeT,Int,Double>> mTabA; 
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
	h5f.addAttribute("fprop", static_cast<Double>(3.141));
	h5f.addGroup("gr1");
	h5f.getGroup("gr1")->addAttribute("att1", String("text"));
	h5f.addGroup("gr2");
	h5f.addGroup("foo");
	h5f.addGroup("bar");
	h5f.addGroup("moregroups");
	auto moregroups = h5f.getGroup("moregroups");
	moregroups->open().addGroup("evenmore");
	auto evenmore = moregroups->getGroup("evenmore");
	evenmore->open();
	evenmore->addAttribute("moreatt", static_cast<Int>(12));
	evenmore->addGroup("we");
	evenmore->getGroup("we")->addAttribute("wex", static_cast<Int>(9));
	evenmore->getGroup("we")->addAttribute("xy", String("xys"));
	evenmore->addGroup("need");
	evenmore->getGroup("need")->addAttribute("wex", static_cast<Int>(7));
	evenmore->addGroup("more");
	evenmore->getGroup("more")->addAttribute("wex", static_cast<Int>(4));
	evenmore->addGroup("groups");
	evenmore->getGroup("groups")->addAttribute("wex", static_cast<Int>(2));
	EXPECT_EQ(h5f.get().size(), 5u);
	h5f.close();
    }

    TEST_F(Group_Test, CreateTable)
    {
	File h5f(mFileName, false);
	h5f.open();
	h5f.getGroup("gr1")->open().addTable("tab1", mTabA, mFs);
	h5f.getGroup("gr2")->open().addTable("tab1", mTabA, mFs);
	h5f.getGroup("moregroups")->open().getGroup("evenmore")->open().getGroup("need")->open().addTable("tab1", mTabA, mFs);
	h5f.close();
    }
    
    TEST_F(Group_Test, Read)
    {
	File h5f(mFileName, true);
	h5f.open();
	EXPECT_TRUE(h5f.ro());
	EXPECT_EQ(h5f.get().size(), 5u);
	
	EXPECT_THROW(h5f.getGroup("gr0"), std::runtime_error);
	auto gr1 = h5f.getGroup("gr1");
	gr1->open();
	auto tab = gr1->getTable("tab1");
	DType att = gr1->getAttribute("att1");
	EXPECT_EQ(att.str(), "text");
	VCHECK(tab->path());
	EXPECT_EQ(tab->fields()->size(), 3u);
	EXPECT_EQ(tab->records()->size(), 5u);
	h5f.iter( [](const auto& c) { VCHECK(c->path()); } )();
	h5f.iterRecursive( [](const auto& c) { VCHECK(c->path()); } )();
	h5f.iterRecursive( [](const auto& c) { c->open(); VCHECK(toString(c->getRecursiveAttributes())); } )();
	h5f.iterRecursive( [](const auto& c) {
	    if(c->type() == ContentType::TABLE) { c->open(); VCHECK(toString(c->getRecursiveAttributes())); }
	} )();
	h5f.close();
    }

    TEST_F(Group_Test, ReadTable)
    {
	typedef Tuple<SizeT,Int,Double> RecType;
	
	File h5f(mFileName, true);
	h5f.open();
	auto tab = h5f.getGroup("gr1")->open().getTable("tab1", RecType());
	EXPECT_EQ(tab->fields()->size(), 3u);
	EXPECT_EQ(tab->records()->size(), 5u);
	
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
