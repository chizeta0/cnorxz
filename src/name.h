// -*- C++ -*-

#ifndef __name_h__
#define __name_h__

#include <cstdlib>
#include <string>
#include <ostream>

#include "base_def.h"

namespace MultiArrayTools
{
    class Name
    {
    public:

	DEFAULT_MEMBERS(Name);
	
	template <class... NameTypes>
	Name(const std::string& mainName, const NameTypes&... names);
	
	template <class... NameTypes>
 	Name(char const* mainName, const NameTypes&... names);

	const std::string& own() const;
	const Name& get(size_t n) const;

	void autoName(size_t newSize);
	size_t size() const;
	
    private:
	std::string mMain;
	std::vector<Name> mSub;	
    };

    std::ostream& operator<<(std::ostream& os, const Name& name);
    
}

#include "name.cc"

#endif
