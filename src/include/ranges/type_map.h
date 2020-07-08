
#ifndef __ma_type_map_h__
#define __ma_type_map_h__

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <cassert>

#include "allocator.h"

namespace MultiArrayTools
{

    template <size_t N>
    struct TypeMap
    {
	typedef void type;
        static constexpr size_t size = 0;
    };

    template <typename T>
    struct NumTypeMap
    {
	static size_t num() { assert(0); return 0; };
    };

    template <typename T>
    struct TypeHandle
    {
        static inline void stringCat(vector<char>& out, const vector<T>& in)
        {
            //for(auto& x: in) { std::cout << x << std::endl; }
            const char* scp = reinterpret_cast<const char*>(in.data());
            out.insert(out.end(), scp, scp + in.size() * sizeof(T));
        }

        static inline size_t metaSize(const vector<T>& in)
        {
            return in.size() * sizeof(T);
        }
	
        static inline void metaCat(vector<T>& vec, const char* begin, size_t size)
        {
            const T* tp = reinterpret_cast<const T*>( begin );
            vec.insert(vec.end(), tp, tp + size / sizeof(T));
        }
    };

    template <>
    struct TypeHandle<std::string>
    {
        static inline void stringCat(vector<char>& out, const vector<std::string>& in)
        {
            //for(auto& x: in) { std::cout << x << std::endl; }
            std::string tmp = "";
            for(auto& x: in){
                tmp += x + '\n';
            }
            const char* scp = reinterpret_cast<const char*>(tmp.data());
            out.insert(out.end(), scp, scp + tmp.size());
        }

        static inline size_t metaSize(const vector<std::string>& in)
        {
            size_t out = 0;
            for(auto& x: in){
                out += x.size() + 1;
            }
            return out;
        }
    
        static inline void metaCat(vector<std::string>& vec, const char* begin, size_t size)
        {

            std::string tmp(begin, size);
            //std::cout << tmp << std::endl;
            size_t pos = 0;
            while(pos != tmp.size()){
                std::string es = "\n";
                size_t termpos = tmp.find_first_of(es, pos);
                std::string tmpstr = tmp.substr(pos, termpos-pos);
                vec.push_back(tmpstr);
                pos = termpos + 1;
            }
        }
    };
    
    template <typename T>
    struct TypeHandle<vector<T>>
    {
        static inline void stringCat(vector<char>& out, const vector<vector<T>>& in)
        {
            //for(auto& x: in) { std::cout << x << std::endl; }
            for(auto& x: in){
                const size_t size = x.size();
                const char* ss = reinterpret_cast<const char*>(&size);
                const char* scp = reinterpret_cast<const char*>(x.data());
                out.insert(out.end(), ss, ss + sizeof(size_t));
                out.insert(out.end(), scp, scp + size*sizeof(T));
            }
        }

        static inline size_t metaSize(const vector<vector<T>>& in)
        {
            size_t out = 0;
            for(auto& x: in){
                out += x.size()*sizeof(T);
                out += sizeof(size_t);
            }
            return out;
        }
        
        static inline void metaCat(vector<vector<T>>& vec, const char* begin, size_t size)
        {
            size_t pos = 0;
            while(pos < size){
                const size_t xsize = *reinterpret_cast<const size_t*>(begin+pos);
                pos += sizeof(size_t);
                const T* dbegin = reinterpret_cast<const T*>(begin+pos);
                const T* dend = dbegin+xsize;
                vec.emplace_back(dbegin,dend);
                //vector<T> tmp(begin+pos, begin+npos);
                //vec.push_back(tmp);
                pos += xsize*sizeof(T);
            }
            assert(pos == size);
        }
    };

    template <typename T>
    inline void stringCat(vector<char>& out, const vector<T>& in)
    {
        TypeHandle<T>::stringCat(out,in);
    }

    template <typename T>
    inline size_t metaSize(const vector<T>& in)
    {
        return TypeHandle<T>::metaSize(in);
    }
	
    template <typename T>
    inline void metaCat(vector<T>& vec, const char* begin, size_t size)
    {
        TypeHandle<T>::metaCat(vec,begin,size);
    }


#define XCOMMAX() ,
    
#define include_type(t,n) template <> struct TypeMap<n> { typedef t type; static constexpr size_t size = sizeof(t); }; \
    template <> struct NumTypeMap<t> { inline static size_t num(){ return n; } };

#include "ranges/type_register.h"
    /*
    include_type(size_t,1)
    include_type(int,2)
    include_type(char,3)
    include_type(float,4)
    include_type(double,5)
    include_type(std::string,6)
    include_type(vector<size_t>,101)
    include_type(vector<int>,102)
    include_type(vector<double>,105)
    include_type(vector<std::string>,106)
    include_type(std::array<size_t XCOMMAX() 2>,201)
    include_type(std::array<int XCOMMAX() 2>,202)
    include_type(std::array<double XCOMMAX() 2>,205)
    include_type(std::array<size_t XCOMMAX() 3>,301)
    include_type(std::array<int XCOMMAX() 3>,302)
    include_type(std::array<double XCOMMAX() 3>,305)
    include_type(std::array<size_t XCOMMAX() 4>,401)
    include_type(std::array<int XCOMMAX() 4>,402)
    include_type(std::array<double XCOMMAX() 4>,405)
    include_type(std::array<int XCOMMAX() 5>,502)
    include_type(std::array<int XCOMMAX() 6>,602)
    include_type(std::array<int XCOMMAX() 7>,702)
    include_type(std::array<int XCOMMAX() 8>,802)
    include_type(std::array<int XCOMMAX() 9>,902)
    */
#undef include_type

    inline size_t sizeFromTypeNum(size_t tn)
    {
        if(tn == 1){ return sizeof(size_t); }
        else if(tn == 2){ return sizeof(int); }
        else if(tn == 3){ return sizeof(char); }
        else if(tn == 4){ return sizeof(float); }
        else if(tn == 5){ return sizeof(double); }
        else if(tn == 6){ return sizeof(std::string); }
        else if(tn > 6 and tn < 100){ assert(0); }
        else if(tn >= 100 and tn < 200) { return sizeof(std::vector<char>); }
        else { const size_t nx = tn % 100; return sizeFromTypeNum(nx)*(tn-nx)/100; }
    }
}

#endif

#undef XCOMMAX


#define register_all_types \
    register_type(1) \
    register_type(2) \
    register_type(3) \
    register_type(4) \
    register_type(5) \
    register_type(6) \
    register_type(101) \
    register_type(102) \
    register_type(105) \
    register_type(106) \
    register_type(201) \
    register_type(202) \
    register_type(301) \
    register_type(302) \
    register_type(402) \
    register_type(502) \
    register_type(602) \
    register_type(702) \
    register_type(802) \
    register_type(902)


#ifdef register_type

register_all_types

#endif
