// -*- C++ -*-
/**

   @file include/operation/basic_operations.h
   @brief Basic operations declaration

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
 **/

#ifndef __cxz_basic_operations_h__
#define __cxz_basic_operations_h__

#include "base/base.h"
#include "operation.h"

namespace CNORXZ
{
    // standard operations:
    // unary:
    
    template <class Op>
    constexpr decltype(auto) minus(const COpInterface<Op>& op);

    // binary:
    
    template <class Op1, class Op2>
    constexpr decltype(auto) plus(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2);
    
    template <class Op1, class Op2>
    constexpr decltype(auto) minus(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2);

    template <class Op1, class Op2>
    constexpr decltype(auto) multiplies(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2);

    template <class Op1, class Op2>
    constexpr decltype(auto) divides(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2);

    template <class Op1, class Op2>
    constexpr decltype(auto) modulo(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2);

    // operators for standard operations:
    
    template <class Op>
    constexpr decltype(auto) operator-(const COpInterface<Op>& op);

    template <class Op1, class Op2>
    constexpr decltype(auto) operator+(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2);

    template <class Op1, class Op2>
    constexpr decltype(auto) operator-(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2);

    template <class Op1, class Op2>
    constexpr decltype(auto) operator*(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2);

    template <class Op1, class Op2>
    constexpr decltype(auto) operator/(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2);

    template <class Op1, class Op2>
    constexpr decltype(auto) operator%(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2);

}

#endif
