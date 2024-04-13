#include "db_service.hpp"

#include <cstddef>

#include "logger.hpp"
#include "../utility/paths.hpp"

namespace sm::service {
    std::list<std::list<std::string>> DBService::select(
        std::string_view rawQuery, 
        std::vector<std::string> values,
        const int schemaSize,
        errorCode& error) noexcept {    
        std::list<std::list<std::string>> result;
        try {
            SQLite::Database database(paths::dbPath, SQLite::OPEN_READONLY);
            SQLite::Statement query(database, std::string{ rawQuery });
            bindValues(query, std::move(values));
 
            while (query.executeStep()) {
                std::list<std::string> record;
                for (int i = 0; i < schemaSize; ++i)
                    record.emplace_back(query.getColumn(i).getString());
                result.emplace_back(record);
            }
        } catch (const std::exception& exception) {
            error = errorCode::readError;
        }

        return result;
    }

    int DBService::insert(
        std::string_view rawQuery, 
        std::vector<std::string> values,
        errorCode& error) noexcept {
        int returnValue = ZERO_CHANGES;
        try {
            SQLite::Database database(paths::dbPath, SQLite::OPEN_READWRITE);
            SQLite::Statement query(database, std::string{ rawQuery });
            bindValues(query, std::move(values));
 
            returnValue = query.exec();
        } catch (const std::exception& exception) {
            Logger::instance().err(exception.what());
            error = errorCode::writeError;
        }

        return returnValue;
    }

    void DBService::bindValues(SQLite::Statement& query, std::vector<std::string> values) {
        for (std::size_t i = 0, size = values.size(); i < size; ++i)
            query.bind(i + 1, values[i]);
    }
}
