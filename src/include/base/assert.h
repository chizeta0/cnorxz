/**
   
   @file include/base/assert.h
   @brief warning and error makros

   Definition of macros that can be used for convenient error handling and warnings

   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#include <iostream>
#include <sstream>

#define CXZ_ERRTAG __FILE__ << '@' << __LINE__ << '(' << __func__ << "): error"
#define CXZ_WARNTAG __FILE__ << '@' << __LINE__ << ": warning"
#define CXZ_ERROR(errmsg) {\
    auto mkerr = [&](){ std::stringstream ss; ss << CXZ_ERRTAG << ": " << errmsg << std::flush; return ss.str(); }; \
    throw std::runtime_error(mkerr()); }

#define CXZ_WARNING(errmsg) {\
    std::cerr << CXZ_WARNTAG << ": " << errmsg << std::endl; }

#define CXZ_ASSERT(statement, errmsg) if(not (statement)) { CXZ_ERROR(errmsg); }
