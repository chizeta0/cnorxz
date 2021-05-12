
#ifndef __range_helper_h__
#define __range_helper_h__

namespace MultiArrayTools
{
    namespace RangeHelper
    {
	template <size_t N, class IndexPack>
	void setIndexPack(IndexPack& iPack, size_t pos)
	{
	    auto& i = *std::get<N>(iPack).get();
	    const size_t ownPos = pos % i.max();
	    i = ownPos;
	    if constexpr(N != 0){
		setIndexPack<N-1>(iPack, (pos - ownPos) / i.max() );
	    }
	}
	
    } // namespace RangeHelper
} // namespace MultiArrayTools

#endif
