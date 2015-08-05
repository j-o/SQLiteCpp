/**
 * @file    Exception.h
 * @ingroup SQLiteCpp
 * @brief   Encapsulation of the error message from SQLite3 on a std::runtime_error.
 *
 * Copyright (c) 2012-2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stdexcept>
#include <string>


/// Compatibility with non-clang compilers.
#ifndef __has_feature
#define __has_feature(x) 0
#endif


// Detect whether the compiler supports C++11 noexcept exception specifications.
#if (  defined(__GNUC__) && ((__GNUC__ == 4 && __GNUC_MINOR__ >= 7) || (__GNUC__ > 4)) \
    && defined(__GXX_EXPERIMENTAL_CXX0X__))
 // GCC 4.7 and following have noexcept
#elif defined(__clang__) && __has_feature(cxx_noexcept)
 // Clang 3.0 and above have noexcept
#elif defined(_MSC_VER) && _MSC_VER > 1800
 // Visual Studio 2015 and above have noexcept
#else
 // Visual Studio 2013 does not support noexcept, and "throw()" is deprecated by C++11
#define noexcept
#endif


struct sqlite3;
struct sqlite3_stmt;


namespace SQLite
{


///**
// * @brief Encapsulation of the error message from SQLite3, based on std::runtime_error.
// */
class Exception : public std::runtime_error
{
public:
    explicit Exception(const char* message = "");

    virtual const char* what() const noexcept override;


protected:
    std::string m_what;
};


class SqlException : public Exception
{
public:
    explicit SqlException(sqlite3* database, const std::string& query = "", const std::string& message = "");
    explicit SqlException(sqlite3_stmt* statement, const std::string& message = "");
};


class BindException : public Exception
{
public:
    BindException(sqlite3_stmt* statement, const std::string& parameter, int resultCode);
    BindException(sqlite3_stmt* statement, int parameter, int resultCode);
};


}  // namespace SQLite
