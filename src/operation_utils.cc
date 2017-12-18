
#include "operation_utils.h"

namespace MultiArrayTools
{

    void seekIndexInst(std::shared_ptr<VIWB> i, std::vector<std::shared_ptr<VIWB> >& ivec)
    {
	for(size_t inum = 0; inum != i->rangePtr()->dim(); ++inum){
	    auto ii = i->getPtr(inum);
	    if(ii->type() == IndexType::MULTI or
	       ii->type() == IndexType::CONT){
		seekIndexInst(ii, ivec);
	    }
	    ivec.push_back(ii);
	}
    }

    BTSS getBlockType(std::shared_ptr<VIWB> i,
		      std::shared_ptr<VIWB> j,
		      bool first, size_t higherStepSize)
    {
	// returning BlockType and step size is redundant (change in the future)
	// stepSize == 0 => VALUE
	// stepSize == 1 => BLOCK
	// stepSize > 1 => SPLIT :)
	BTSS out(BlockType::VALUE, 0);
	size_t lastNum = i->rangePtr()->dim();

	for(size_t inum = 0; inum != lastNum; ++inum){
	    auto ii = i->getPtr(inum);
	    if(ii->getPtrNum() == j->getPtrNum()){
		
		if(inum == lastNum - 1 and first){
		    out = BTSS(BlockType::BLOCK, 1);
		}
		else {
		    first = false;
		    out = BTSS(BlockType::SPLIT, i->getStepSize(inum) * higherStepSize + out.second);
		}
		continue;
	    }
	    
	    if(ii->type() == IndexType::MULTI or
	       ii->type() == IndexType::CONT){

		BTSS tmp = getBlockType(ii, j, inum == lastNum - 1, i->getStepSize(inum) * higherStepSize);
		if(tmp.first != BlockType::VALUE){
		    out = tmp;
		}
	    }
	}
	return out;
    }

    size_t getBTNum(const std::vector<BTSS>& mp, BlockType bt)
    {
	size_t out = 0;
	for(auto& xx: mp){
	    if(xx.first == bt){
		++out;
	    }
	}
	return out;
    }

    void minimizeAppearanceOfType(std::map<std::shared_ptr<VIWB>, std::vector<BTSS> >& mp,
				  BlockType bt)
    {
	size_t minNum = getBTNum( mp.begin()->second, bt );
	for(auto& mm: mp){
	    size_t tmp = getBTNum( mm.second, bt );
	    if(tmp < minNum){
		minNum = tmp;
	    }
	}

	for(auto mit = mp.begin(); mit != mp.end(); ){
	    size_t tmp = getBTNum( mit->second, bt );
	    if(tmp > minNum){
		mit = mp.erase(mit);
	    }
	    else {
		++mit;
	    }
	}

    }
    


} // end namespace MultiArrayTools
