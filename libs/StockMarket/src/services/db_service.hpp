#pragma once

#include <vector>
#include <list>
#include <string_view>

#include <SQLiteCpp/SQLiteCpp.h>

namespace sm::service {
    /*!
        \brief Отвечает за обращения к БД
    */
    class DBService {
    public:
        enum class errorCode {
            noError,
            readError,
            writeError
        };

        DBService() = default;

        ~DBService() = default;
        
        static std::list<std::list<std::string>> select(
            std::string_view rawQuery, 
            std::vector<std::string> values,
            const int schemaSize,
            errorCode& error) noexcept;

        static int insert(
            std::string_view rawQuery, 
            std::vector<std::string> values,
            errorCode& error) noexcept;

        inline static constexpr int ZERO_CHANGES = 0;
        inline static const std::string NULL_VALUE = "NULL";

    private:
        static void bindValues(SQLite::Statement& query, std::vector<std::string> values);
    };
}