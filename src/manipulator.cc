// -*- C++ -*-

#include "manipulator.h"

namespace MultiArrayTools
{

    template <typename T>
    void ManipulatorBase<T>::setup(std::vector<T>& targetRef, size_t begin, size_t end)
    {
	mBegin = begin;
	mEnd = end;
	mTargetPtr = &targetRef;
    }

    template <typename T>
    void ManipulatorBase<T>::reset()
    {
	mLastPos = 0;
    }

    template <typename T>
    BinReader<T>::BinReader(const std::string& fname, size_t readBegin):
	mFs(fname, std::fstream::in | std::fstream::binary),
	mReadBegin(readBegin) {}

    template <typename T>
    BinReader<T>::~BinReader()
    {
	mFs.close();
    }
    
    template <typename T>
    void BinReader<T>::execute()
    {
	const size_t readSize = MB::mEnd - MB::mBegin;
	const size_t blockSize = sizeof(T) * readSize;
	char* buffer = new char[blockSize];

	mFs.seekg(mReadBegin + MB::mLastPos, mFs.beg);
	mFs.read(buffer, blockSize);
	MB::mLastPos += blockSize;
	
	T* content = reinterpret_cast<T*>( buffer );

	if(MB::mTargetPtr->size() < MB::mEnd){
	    assert(0);
	    // throw
	}
	
	std::copy(&content[0], &content[readSize], MB::mTargetPtr->begin() + MB::mBegin);

	delete[] buffer;	
    }

}
