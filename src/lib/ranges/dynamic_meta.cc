
#include <cstdlib>
#include <vector>
#include "ranges/dynamic_meta.h"
#include "ranges/ranges_header.cc.h"

namespace MultiArrayTools
{

    /*********************
     *   DynamicMetaT    *
     *********************/

    bool DynamicMetaT::operator==(const DynamicMetaT& in) const
    {
        if(in.size() != mMeta.size()) { return false; }
        for(size_t i = 0; i != mMeta.size(); ++i){
            if(in[i].second != mMeta[i].second) { return false; }
            for(size_t j = 0; j != mMeta[i].second; ++j){
                if(in[i].first[j] != mMeta[i].first[j]) { return false; }
            }
        }
        return true;
    }
    
    bool DynamicMetaT::operator!=(const DynamicMetaT& in) const
    {
        return not operator==(in);
    }
    
    size_t DynamicMetaT::size() const
    {
        return mMeta.size();
    }
    
    DynamicMetaElem& DynamicMetaT::operator[](size_t pos)
    {
        return mMeta[pos];
    }
    
    const DynamicMetaElem& DynamicMetaT::operator[](size_t pos) const
    {
        return mMeta[pos];
    }


    
} // namespace MultiArrayTools
