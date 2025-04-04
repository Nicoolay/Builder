#include <iostream>
#include <string>
#include <vector>
#include <map>

class SqlSelectQueryBuilder {
public:
    class Builder {
    public:
        Builder& AddColumn(const std::string& column) {
            columns_.push_back(column);
            return *this;
        }

        Builder& AddFrom(const std::string& table) {
            table_ = table;
            return *this;
        }

        Builder& AddWhere(const std::string& column, const std::string& value) {
            where_clauses_[column] = value;
            return *this;
        }

        SqlSelectQueryBuilder Build() const {
            return SqlSelectQueryBuilder(columns_, table_, where_clauses_);
        }

    private:
        std::vector<std::string> columns_;
        std::string table_;
        std::map<std::string, std::string> where_clauses_;
    };

    std::string BuildQuery() const {
        if (table_.empty()) {
            return ""; 
        }

        std::string query = "SELECT ";

        if (columns_.empty()) {
            query += "*";
        }
        else {
            for (size_t i = 0; i < columns_.size(); ++i) {
                if (i > 0) query += ", ";
                query += columns_[i];
            }
        }

        query += " FROM " + table_;

        if (!where_clauses_.empty()) {
            query += " WHERE ";
            bool first = true;
            for (const auto& pair : where_clauses_) {
                if (!first) query += " AND ";
                query += pair.first + "=" + pair.second;
                first = false;
            }
        }

        query += ";";
        return query;
    }

private:
    SqlSelectQueryBuilder(const std::vector<std::string>& columns, const std::string& table, const std::map<std::string, std::string>& where_clauses)
        : columns_(columns), table_(table), where_clauses_(where_clauses) {
    }

    std::vector<std::string> columns_;
    std::string table_;
    std::map<std::string, std::string> where_clauses_;
};

int main() {
    SqlSelectQueryBuilder query_builder = SqlSelectQueryBuilder::Builder()
        .AddColumn("name")
        .AddColumn("phone")
        .AddFrom("students")
        .AddWhere("id", "42")
        .AddWhere("name", "John")
        .Build();

    std::cout << query_builder.BuildQuery() << std::endl;
    return 0;
}
