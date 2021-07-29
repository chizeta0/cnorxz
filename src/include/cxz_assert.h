
#include <iostream>
#include <sstream>

#define MA_ERRTAG __FILE__ << '@' << __LINE__ << '(' << __func__ << "): error"
#define MA_WARNTAG __FILE__ << '@' << __LINE__ << ": warning"
#define MA_ERROR(errmsg) {\
    std::stringstream ss;\
    ss << MA_ERRTAG << ": " << errmsg << std::flush;\
    throw std::runtime_error(ss.str()); }

#define MA_WARNING(errmsg) {\
    std::cerr << MA_WARNTAG << ": " << errmsg << std::endl; }

#define MA_ASSERT(statement, errmsg) if(not (statement)) { MA_ERROR(errmsg); }
