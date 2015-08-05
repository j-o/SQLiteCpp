/**
* @file    Database.cpp
* @ingroup SQLiteCpp
* @brief   Management of a SQLite Database Connection.
*
* Copyright (c) 2012-2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
*
* Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
* or copy at http://opensource.org/licenses/MIT)
*/

#include <SQLiteCpp/Exception.h>

#include <sstream>

#include <sqlite3.h>


namespace SQLite
{


Exception::Exception(const char* message)
: std::runtime_error(message) {
}


const char* Exception::what() const noexcept{
    return m_what.c_str();
}


SqlException::SqlException(sqlite3* database, const std::string& query, const std::string& message) {
    std::stringstream what;

    const auto errorCode = sqlite3_extended_errcode(database);
    const auto errorString = sqlite3_errstr(errorCode);
    const auto errorMessage = sqlite3_errmsg(database);

    what << errorString << " (" << errorCode << "): " << errorMessage;

    if (!message.empty()) {
        what << " (" << message << ")";
    }

    if (!query.empty()) {
        what << " for query \"" << query << "\"";
    }

    m_what = what.str();
}


SqlException::SqlException(sqlite3_stmt* statement, const std::string& message)
: SqlException(sqlite3_db_handle(statement), sqlite3_sql(statement), message) {
}


BindException::BindException(sqlite3_stmt* statement, const std::string& parameter, int resultCode) {
    const auto errorString = sqlite3_errstr(resultCode);
    const auto query = sqlite3_sql(statement);

    std::stringstream what;
    what << errorString << " (" << resultCode << ") for parameter \"" << parameter << "\" of query \"" << query << "\"";
    m_what = what.str();
}


BindException::BindException(sqlite3_stmt* statement, int parameter, int resultCode)
: BindException(statement, std::to_string(parameter), resultCode) {
}

}  // namespace SQLite
