
#ifndef __dynamic_meta_h__
#define __dynamic_meta_h__

#include <cstdlib>
#include <utility>

namespace MultiArrayTools
{

    typedef std::pair<const char*,size_t> DynamicMetaElem;
    
    class DynamicMetaT
    {
    private:
        std::vector<DynamicMetaElem> mMeta;

    public:
        DynamicMetaT() = default;
        DynamicMetaT(const DynamicMetaT& in) = default;
        DynamicMetaT(DynamicMetaT&& in) = default;
        DynamicMetaT& operator=(const DynamicMetaT& in) = default;
        DynamicMetaT& operator=(DynamicMetaT&& in) = default;

        template <typename... Us>
        DynamicMetaT(const std::tuple<Us...>& meta)
        {
            
        }
        
        bool operator==(const DynamicMetaT& in) const;
        bool operator!=(const DynamicMetaT& in) const;

        size_t size() const;
        
        DynamicMetaElem& operator[](size_t pos);
        const DynamicMetaElem& operator[](size_t pos) const;
    };


} // namespace MultiArrayTools

#endif
