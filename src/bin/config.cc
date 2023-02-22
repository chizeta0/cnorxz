
#include <cstdlib>
#include <iostream>
#include <map>
#include <functional>
#include "base/config.h"

using CNORXZ::String;
using CNORXZ::SizeT;
typedef std::map<String,std::function<String(void)>> CMapT;

auto configMap()
{
    CMapT m;
    m["--version"] = CNORXZ::Config::version;
    m["--commit"] = CNORXZ::Config::commit;
    m["--flags"] = CNORXZ::Config::flags;
    return m;
}

void printUsage(const String& prog, const CMapT& cm)
{
    std::cout << "usage: " << prog << " ";
    for(const auto& x: cm){
	std::cout << "[" << x.first << "] ";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    const String prog(argv[0]);
    const auto cm = configMap();
    if(argc == 1){
	printUsage(prog, cm);
    }
    try {
	for(SizeT i = 1; i != static_cast<SizeT>(argc); ++i){
	    std::cout << cm.at( String(argv[i]) )() << std::endl;
	}
    }
    catch(const std::out_of_range& e){
	std::cerr << "caught exception: " << e.what() << std::endl;
	printUsage(prog, cm);
	return 1;
    }
    catch(...){
	std::cerr << "caught generic exception" << std::endl;
    }
    
    return 0;
}
