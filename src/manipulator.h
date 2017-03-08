// -*- C++ -*-

#ifndef __manipulator_h__
#define __manipulator_h__

#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>

#include "base_def.h"

namespace MultiArrayTools
{

    // some kind of input stream or sth similar...
    template <typename T>
    class ManipulatorBase
    {
    public:
	virtual ~ManipulatorBase() {}
	
	virtual void setup(std::vector<T>& targetRef, size_t begin, size_t end);
	virtual void execute(size_t pos) = 0;
	virtual void reset();
	
    protected:

	size_t mLastPos = 0;
	size_t mBegin = 0;
	size_t mEnd = 0;
	std::vector<T>* mTargetPtr = nullptr;
    };

    template <typename T>
    class BinReader : public ManipulatorBase<T>
    {
    public:
	typedef ManipulatorBase<T> MB;

	BinReader(const std::string& fname, size_t readBegin = 0);
	virtual ~BinReader();
	
	
	virtual void execute() override;

    private:
	size_t mReadBegin;
	std::fstream mFs;
    };

}

#include "manipulator.cc"

#endif
