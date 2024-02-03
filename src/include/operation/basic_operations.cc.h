// -*- C++ -*-
/**

   @file include/operation/basic_operations.cc.h
   @brief Basic operations implementation

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
 **/

#ifndef __cxz_basic_operations_cc_h__
#define __cxz_basic_operations_cc_h__

#include "basic_operations.h"

namespace CNORXZ
{
    /*==================================+
     |   standard operatrions (unary)   |
     +==================================*/

    template <class Op>
    constexpr decltype(auto) minus(const COpInterface<Op>& op)
    {
	return operation( [](const auto& a) { return -a; }, op.THIS() );
    }

    /*===================================+
     |   standard operatrions (binary)   |
     +===================================*/

    template <class Op1, class Op2>
    constexpr decltype(auto) plus(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2)
    {
	return operation( [](const auto& a, const auto& b) { return a + b; },
			  op1.THIS(), op2.THIS() );
    }

    template <class Op1, class Op2>
    constexpr decltype(auto) minus(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2)
    {
	return operation( [](const auto& a, const auto& b) { return a - b; },
			  op1.THIS(), op2.THIS() );
    }

    template <class Op1, class Op2>
    constexpr decltype(auto) multiplies(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2)
    {
	return operation( [](const auto& a, const auto& b) { return a * b; },
			  op1.THIS(), op2.THIS() );
    }

    template <class Op1, class Op2>
    constexpr decltype(auto) divides(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2)
    {
	return operation( [](const auto& a, const auto& b) { return a / b; },
			  op1.THIS(), op2.THIS() );
    }

    template <class Op1, class Op2>
    constexpr decltype(auto) modulo(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2)
    {
	return operation( [](const auto& a, const auto& b) { return a % b; },
			  op1.THIS(), op2.THIS() );
    }

    /*=======================================+
     |   operators for standard operations   |
     +=======================================*/

    template <class Op>
    constexpr decltype(auto) operator-(const COpInterface<Op>& op)
    {
	return minus(op);
    }

    template <class Op1, class Op2>
    constexpr decltype(auto) operator+(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2)
    {
	return plus(op1, op2);
    }

    template <class Op1, class Op2>
    constexpr decltype(auto) operator-(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2)
    {
	return minus(op1, op2);
    }

    template <class Op1, class Op2>
    constexpr decltype(auto) operator*(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2)
    {
	return multiplies(op1, op2);
    }

    template <class Op1, class Op2>
    constexpr decltype(auto) operator/(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2)
    {
	return divides(op1, op2);
    }

    template <class Op1, class Op2>
    constexpr decltype(auto) operator%(const COpInterface<Op1>& op1, const COpInterface<Op2>& op2)
    {
	return modulo(op1, op2);
    }
}

#endif
