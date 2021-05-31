#pragma once
#include <iostream>
#include <stdio.h>
#include <string>

#ifndef NDEBUG
#   define KIT_ASSERT(Expr, Msg) \
    __KIT_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#   define ASSERT_EQUAL(A, B, Msg) \
    __ASSERT_EQUAL(#A, #B, A, B, __FILE__, __LINE__, Msg);
#else
#   define KIT_ASSERT(Expr, Msg) ;
#endif

inline void __KIT_Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg)
{
    if (!expr)
    {
        std::cerr << "Assert failed:\t" << msg << "\n"
            << "Expected:\t" << expr_str << "\n"
            << "Source:\t\t" << file << ", line " << line << "\n";
        abort();
    }
}

inline void __KIT_Assert(const char* expr_str, bool expr, const char* file, int line, const std::string msg)
{
    if (!expr)
    {
        std::cerr << "Assert failed:\t" << msg << "\n"
            << "Expected:\t" << expr_str << "\n"
            << "Source:\t\t" << file << ", line " << line << "\n";
        abort();
    }
}