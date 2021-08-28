#pragma once

#include <algorithm>
#include <mutex>
#include <atomic>
#include <tuple>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include <boost/algorithm/string.hpp>

#include "sqlite3.h"

const std::string LEFT_TABLE{"A"};
const std::string RIGHT_TABLE{"B"};

class DBResultHandler
{
public:
    virtual int handle_row(int id, std::string name_left, std::string name_right) = 0;
    virtual ~DBResultHandler() = default;
};

class DBManager
{
    static DBManager* m_instance;
    static std::once_flag m_instance_once;

    std::atomic_bool m_is_opened{false};
    sqlite3* m_db{nullptr};

public:

    static DBManager& get()
    {
        std::call_once(m_instance_once, [](){ m_instance = new DBManager; });
        return *m_instance;
    }

    bool is_opened() const
    {
        return m_is_opened ? bool(m_db) : false;
    }

    bool open_db(std::string db_name)
    {
        if (!is_opened())
            m_is_opened = (sqlite3_open(db_name.c_str(), &m_db) == 0);

        return m_is_opened;
    }

    bool create_table(std::string table_name)
    {
        return execute_query("CREATE TABLE " + table_name + " (id INTEGER PRIMARY KEY UNIQUE, name TEXT NOT NULL);");
    }

    bool insert(std::string table_name, int id, std::string name)
    {
        return execute_query("INSERT INTO " + table_name + " VALUES (" + std::to_string(id) + ",'" + name + "');");
    }

    bool truncate(std::string table_name)
    {
        return execute_query("DELETE FROM " + table_name + ";");
    }

    bool intersection(DBResultHandler* res_handler)
    {
        std::stringstream query;

        query << "SELECT" << ' ';
            query << LEFT_TABLE  << ".id AS '"   << LEFT_TABLE  << "_id'"   << ',';
            query << LEFT_TABLE  << ".name AS '" << LEFT_TABLE  << "_name'" << ',';
            query << RIGHT_TABLE << ".id AS '"   << RIGHT_TABLE << "_id'"   << ',';
            query << RIGHT_TABLE << ".name AS '" << RIGHT_TABLE << "_name'" << ' ';
        query << "FROM " << LEFT_TABLE << ' ';
            query << "INNER JOIN " << RIGHT_TABLE << " on " << LEFT_TABLE << ".id = " << RIGHT_TABLE << ".id";

        return execute_query(query.str(), res_handler);
    }

    bool symmetric_difference(DBResultHandler* res_handler)
    {
        std::stringstream query;

        query << "SELECT" << ' ';
            query << "id AS '" << LEFT_TABLE  << "_id'"   << ',';
            query << "name AS '" << LEFT_TABLE  << "_name'" << ' ';
        query << "FROM " << LEFT_TABLE << ' ';
            query << "WHERE " << LEFT_TABLE << "_id NOT IN ( SELECT id AS " << LEFT_TABLE << "_id FROM " << RIGHT_TABLE << ")";

        query << ";";

        query << "SELECT" << ' ';
            query << "id AS '" << RIGHT_TABLE << "_id'"   << ',';
            query << "name AS '" << RIGHT_TABLE << "_name'" << ' ';
        query << "FROM " << RIGHT_TABLE << ' ';
            query << "WHERE " << RIGHT_TABLE << "_id NOT IN ( SELECT id AS " << RIGHT_TABLE << "_id FROM " << LEFT_TABLE << ")";

        return execute_query(query.str(), res_handler);
    }

private:

    DBManager()
    {
        open_db("hw10.db");
        create_table(LEFT_TABLE);
        create_table(RIGHT_TABLE);
    }

    DBManager(const DBManager&) = delete;
    DBManager& operator= (const DBManager&) = delete;

    ~DBManager()
    {
        if (m_db)
        {
            sqlite3_close(m_db);
            sqlite3_free(m_db);
        }
    }

    bool execute_query(std::string query, DBResultHandler* res_handler = nullptr)
    {
        auto db_res_callback = [](void* context, int columns, char** data, char** names)
        {
            if (context && columns && data && names)
            {
                int id_buf;
                std::string left_name_buf;
                std::string right_name_buf;

                for (int i = 0; i < columns; ++i)
                {
                    std::string str_name{names[i]};
                    boost::trim(str_name);

                    std::string str_data{data[i]};
                    boost::trim(str_data);

                    if (str_name.find("_id") != std::string::npos)
                    {
                        id_buf = std::stoi(str_data);
                    }
                    else if (str_name.find(LEFT_TABLE+"_") != std::string::npos)
                    {
                        left_name_buf = str_data;
                    }
                    else if (str_name.find(RIGHT_TABLE+"_") != std::string::npos)
                    {
                        right_name_buf = str_data;
                    }
                }

                return static_cast<DBResultHandler*>(context)->handle_row(id_buf, left_name_buf, right_name_buf);
            }

            return -1;
        };

        return is_opened() ? (sqlite3_exec(m_db, query.c_str(), db_res_callback, res_handler, nullptr) == 0) : false;
    }
};

DBManager* DBManager::m_instance;
std::once_flag DBManager::m_instance_once;
