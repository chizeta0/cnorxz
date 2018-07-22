
#ifndef __ma_type_map_h__
#define __ma_type_map_h__

#include <string>

namespace MultiArrayTools
{

    template <size_t N>
    struct TypeMap
    {
	typedef void type;
    };

    template <typename T>
    struct NumTypeMap
    {
	static constexpr size_t num = 0;
    };

    template <typename T>
    void stringCat(std::vector<char>& out, const std::vector<T>& in)
    {
	const char* scp = reinterpret_cast<const char*>(in.data());
	out.insert(out.end(), scp, scp + in.size() * sizeof(T));
    }
    
    template <typename T>
    void metaCat(std::vector<T>& vec, char* begin, size_t size)
    {
	T* tp = reinterpret_cast<T*>( begin );
	vec.insert(vec.end(), tp, tp + size / sizeof(T));
    }

    template <>
    void stringCat<std::string>(std::vector<char>& out, const std::vector<std::string>& in)
    {
	std::string tmp = "";
	for(auto& x: in){
	    tmp += x + '\0';
	}
	const char* scp = reinterpret_cast<const char*>(tmp.data());
	out.insert(out.end(), scp, scp + tmp.size());
    }
    
    template <>
    void metaCat<std::string>(std::vector<std::string>& vec, char* begin, size_t size)
    {
	size_t pos = 0;
	std::string tmp(begin, size);
	while(pos != size){
	    size_t termpos = tmp.find_first_of('\0');
	    vec.push_back(tmp.substr(pos,termpos));
	    pos = termpos + 1; // skip '\0'
	}
    }
    
#define include_type(t,n) template <> struct TypeMap<n> { typedef t type; };\
    template <> struct NumTypeMap<t> { static constexpr size_t num = n; };

    include_type(size_t,1)
    include_type(int,2)
    include_type(char,3)
    include_type(float,4)
    include_type(double,5)
    include_type(std::string,6)
   
#undef include_type
}
#endif

#ifdef register_type

register_type(1)
register_type(2)
register_type(3)
register_type(4)
register_type(5)
register_type(6)

#endif
