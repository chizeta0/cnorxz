// -*- C++ -*-
/**
   
   @file include/base/assert.h
   @brief warning and error makros

   Definition of macros that can be used for convenient error handling and warnings

   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#include <iostream>
#include <sstream>

/** library error tag */
#define CXZ_ERRTAG __FILE__ << '@' << __LINE__ << '(' << __func__ << "): error"

/** library warning tag */
#define CXZ_WARNTAG __FILE__ << '@' << __LINE__ << ": warning"

/** throw error
    @param errmsg error message
 */
#define CXZ_ERROR(errmsg) {						\
    auto mkerr = [&](){ std::stringstream ss; ss << CXZ_ERRTAG << ": " << errmsg << std::flush; return ss.str(); }; \
    throw std::runtime_error(mkerr()); }

/** print warning
    @param errmsg warning message
 */
#define CXZ_WARNING(errmsg) {\
    std::cerr << CXZ_WARNTAG << ": " << errmsg << std::endl; }

/** throw error if given statement is not fulfilled
    @param statement statement to be checked
    @param errmsg error message
 */
#define CXZ_ASSERT(statement, errmsg) if(not (statement)) { CXZ_ERROR(errmsg); }
