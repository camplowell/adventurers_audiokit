#pragma once
#include <iostream>
#include <stdio.h>
#include <string>

/*
 * Copyright (c) 2021 Lowell Camp
 * 
 * This file is part of the Adventurer's Audiokit,
 * which is licensed under the Mozilla Public License 2.0
 * See https://opensource.org/licenses/MPL-2.0
 */


/**
 * @brief Very simple debugging enhancements,
 * primarily making "assert" more flexible
 */
#ifndef NDEBUG
#   define KIT_ASSERT(Expr, Msg) \
    __KIT_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
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