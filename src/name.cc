
#include "name.h"

namespace MultiArrayTools
{

    namespace
    {

	void giveNames(std::vector<Name>& nvec)
	{
	    nvec.clear();
	}

	void giveNames(std::vector<Name>& nvec, const Name& name)
	{
	    nvec.push_back(name);
	}

	template <class... NameTypes>
	void giveNames(std::vector<Name>& nvec, const Name& name1, const Name& name2, const NameTypes&... names)
	{
	    nvec.push_back(name1);
	    giveNames(nvec, name2, names...);
	}

	
    }
    
    template <class... NameTypes>
    Name::Name(const std::string& mainName, const NameTypes&... names) : mMain(mainName)
    {
	mSub.reserve(sizeof...(NameTypes));
	giveNames(mSub, names...);
    }

    template <class... NameTypes>
    Name::Name(char const* mainName, const NameTypes&... names) : mMain(mainName)
    {
	mSub.reserve(sizeof...(NameTypes));
	giveNames(mSub, names...);
    }
    
    const std::string& Name::own() const
    {
	return mMain;
    }
    
    const Name& Name::get(size_t n) const
    {
	return mSub.at(n);
    }

    void Name::autoName(size_t newSize)
    {
	size_t oldSize = mSub.size();
	mSub.resize(newSize);
	if(oldSize < newSize){
	    for(size_t i = oldSize; i != newSize; ++i){
		mSub[i] = Name( mMain + std::to_string( i ) );
	    }
	}
    }

    size_t Name::size() const
    {
	return mSub.size();
    }
    
}
