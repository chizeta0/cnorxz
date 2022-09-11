
#include <iostream>
#include <sstream>

#define CXZ_ERRTAG __FILE__ << '@' << __LINE__ << '(' << __func__ << "): error"
#define CXZ_WARNTAG __FILE__ << '@' << __LINE__ << ": warning"
#define CXZ_ERROR(errmsg) {\
    std::stringstream ss;\
    ss << CXZ_ERRTAG << ": " << errmsg << std::flush;\
    throw std::runtime_error(ss.str()); }

#define CXZ_WARNING(errmsg) {\
    std::cerr << CXZ_WARNTAG << ": " << errmsg << std::endl; }

#define CXZ_ASSERT(statement, errmsg) if(not (statement)) { CXZ_ERROR(errmsg); }
