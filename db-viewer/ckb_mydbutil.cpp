#include "ckb_mydbutil.h"

#include <mysql-cppconn/mysqlx/xdevapi.h>

#include <map>
#include <variant>
#include <iostream>
#include <functional>
#include <fstream>
#include <cctype>


const std::string ckb::MyDBUtil::sql_file = "/Documents/sql.txt";



namespace {



// # conn1
// conn1.dbtype=mysql
// conn1.url=localhost
// conn1.port=33060
// conn1.user=root
// conn1.password=123456


struct DB_conn {
    std::string type;
    std::string url;
    std::string port;
    std::string user;
    std::string pwd;
    std::variant<mysqlx::Session*> conn;   // <any> ?

    bool setValue(std::string& prop, std::string& value) {
        if (map2.find(prop) == map2.end()) {
            std::cerr<<"prop not exist: "<<prop<<std::endl;
            return false;
        }

        map2[prop](this, value);
        return true;
    }

    bool generate_conn() {
        // std::cout<<"start to generate conn"<<std::endl;
        if (map3.find(this->type) == map3.end()) {
            std::cerr<<"type not exist: "<<this->type<<std::endl;
            return false;
        }
        // map3[this->type](this);  // const map cannot use operator[]
        map3.at(this->type)(this);
        return true;
    }

private:

    // static must init at outer
    static const std::map<std::string, std::function<void(DB_conn*)>> map3;

    static std::map<std::string, std::function<void(DB_conn*, std::string&)>> map2;
};

const std::map<std::string, std::function<void(DB_conn*)>> DB_conn::map3 = {
    {
     "mysql", [](DB_conn* conn){
         std::cout<<"start to conn db "<<", "<<conn->url<<", "<<conn->port<<", "<<conn->user<<std::endl;
         conn->conn = new mysqlx::Session(conn->url, std::stoi(conn->port), conn->user, conn->pwd);
     }
    },
};

std::map<std::string, std::function<void(DB_conn*, std::string&)>> DB_conn::map2 = {
    {"type", [](DB_conn* conn, std::string& value){ conn->type = value; }},
    {"url", [](DB_conn* conn, std::string& value){ conn->url = value; }},
    {"port", [](DB_conn* conn, std::string& value){ conn->port = value; }},
    {"user", [](DB_conn* conn, std::string& value){ conn->user = value; }},
    {"pwd", [](DB_conn* conn, std::string& value){ conn->pwd = value; }},
};

// singleton!!
std::map<std::string, DB_conn> conn_map; // non-POD

// std::map<std::string, DB_conn*> conn_map_2; // POD qt's hints are strange..   only this, won't non-POD.

// std::map<std::string, std::function<void(DB_conn*, std::string&)>> map2 = {
//     {"type", [](DB_conn* conn, std::string& value){ conn->type = value; }},
//     {"url", [](DB_conn* conn, std::string& value){ conn->url = value; }}
// };

void init_conn_map() {

    std::cout<<"\n========== init start ==========="<<std::endl;

    std::ifstream f("db.properties");
    if (f.is_open()) {
        std::string line, name, prop, value;
        int idx_dot, idx_eq;
        while (std::getline(f, line)) {
            if (line.empty() || line[0] == '#')
                continue;

            idx_dot = line.find('.');
            idx_eq = line.find('=');
            name = line.substr(0, idx_dot);
            prop = line.substr(idx_dot + 1, idx_eq - idx_dot - 1);  // (start_index, count)...
            value = line.substr(idx_eq + 1);

            // std::cout<<name<<", "<<prop<<" = "<<value<<std::endl;

            conn_map[name].setValue(prop, value);

            // last_name = name;
        }
        f.close();
    } else {
        std::cerr<<"cannot open file"<<std::endl;
    }

    for (auto& p : conn_map) {
        if (!p.second.generate_conn()) {
            std::cerr<<"generate_conn failed"<<std::endl;
        }
    }

    if (conn_map.empty()) {
        std::cerr<<"no connection!!!!"<<std::endl;
    }

    std::cout<<"========= init end ==========\n"<<std::endl;

}

template<typename T>
T get_conn(std::string& db) {
    if (conn_map.empty())
        init_conn_map();

    return std::get<T>(conn_map[db].conn);  // get<index or type>
}

void sql_wrong_then_u_return(ckb::MyDBResult& res, std::string info) {
    res.first.push_back(info + "~~~");
}

} // anonymous/unnamed namespace end


// for QComboBox.....
std::vector<std::string> ckb::MyDBUtil::get_all_conn_name() {
    std::vector<std::string> vs;
    if (conn_map.empty())
        init_conn_map();

    for (auto& [key, _] : conn_map) {
        vs.push_back(key);
    }
    return vs;
}

// <columns' name[], datas[][]>
std::pair<std::vector<std::string>, std::vector<std::vector<std::string>>> ckb::MyDBUtil::query_db(std::string& sql, std::string& db) {

    // <column name, vector<db's row>>
    std::pair<std::vector<std::string>, std::vector<std::vector<std::string>>> pvvv;

    mysqlx::Session* session = get_conn<mysqlx::Session*>(db);

    // https://dev.mysql.com/doc/dev/connector-cpp/latest/classmysqlx_1_1abi2_1_1r0_1_1SqlResult.html
    // https://dev.mysql.com/doc/dev/connector-cpp/latest/classmysqlx_1_1abi2_1_1r0_1_1Column.html
    // https://dev.mysql.com/doc/dev/connector-cpp/latest/classmysqlx_1_1abi2_1_1r0_1_1Value.html

    while (!sql.empty() && !std::isprint(sql.back())) {
        sql.pop_back();
    }
    while (!sql.empty() && !std::isalpha(sql[0])) {
        sql.erase(sql.begin());
    }
    for (char& ch : sql) {
        if (ch == '\n' || ch == '\r') {
            ch = ' ';
        }
    }
    std::cout<<sql<<std::endl;
    if (sql.empty()) {
        sql_wrong_then_u_return(pvvv, "u not choose sql");
        return pvvv;
    }

    try {
        mysqlx::SqlResult result = session->sql(sql).execute();

        if (result.getWarningsCount() > 0) {
            sql_wrong_then_u_return(pvvv, result.getWarning(0).getMessage());
            return pvvv;
        }

        if (!result.hasData()) {
            sql_wrong_then_u_return(pvvv, "success && no result");
            return pvvv;
        }

        int sz1 = result.getColumnCount();
        for (const mysqlx::Column& col : result.getColumns()) {
            // std::cout<<col.getColumnName()<<", "<<col.getSchemaName()<<", "<<col.getTableName()<<", "<<col.getTableLabel()<<", "<<col.getColumnLabel()<<std::endl;
            pvvv.first.push_back(col.getColumnName());
        }
        mysqlx::Row r;

        while ((r = result.fetchOne())) {  // row
            std::vector<std::string> vs;
            for (int i = 0; i < sz1; ++i) {  // cell
                mysqlx::Value val = r.get(i);    // r[i]

                // vt.push_back(std::make_tuple(r[0].get<int>(), r[1].get<std::string>(), r[2].get<std::string>(), r[3].get<int>(), r[4].get<std::string>()));

                // val.getType();
                // switch(val.getType()) {
                // // enum Type {
                // //     VNULL , UINT64 , INT64 , FLOAT ,
                // //     DOUBLE , BOOL , STRING , DOCUMENT ,
                // //     RAW , ARRAY
                // // }
                // case mysqlx::Value::Type::INT64:
                // case mysqlx::Value::Type::UINT64:
                //     break;
                // // case mysqlx::BOOL:
                // case mysqlx::Value::STRING:
                //     break;
                // default:
                //     std::cerr<<"not switched mysqlx Value Type: "<<val.getType()<<std::endl;
                //     break;
                // }

                // std::cout<<val<<std::endl;
                // vs.push_back(std::string(val)); // Mysqlx.Error: Value cannot be converted to string

                std::string cvt;
                switch(val.getType()) {
                case mysqlx::Value::Type::VNULL:
                    cvt = "Nil";
                    break;
                case mysqlx::Value::Type::INT64:
                    cvt = std::to_string(int64_t((val)));
                    break;
                case mysqlx::Value::Type::STRING:
                    cvt = std::string(val);
                    break;
                default:
                    cvt = "Un_converted";
                    break;
                }
                vs.push_back(cvt);
            }
            pvvv.second.push_back(vs);
        }
    // } catch (...) {
    } catch (mysqlx::abi2::r0::Error& e) {
        std::cerr<<e.what()<<std::endl;
        std::cerr<<"sql failed"<<std::endl;
    } catch (...) {
        std::cerr<<"sql failed..."<<std::endl;
    }


    return pvvv;
}



/////////////////////////

#ifdef MY_QT_FLAG  // CMakeLists.txt  add_definitions(-DMY_QT_FLAG)
int main2(int argc, char** argv) {
#else
int main(int argc, char** argv) {
#endif

    // std::string sql = "select * from aaa.file where id between 3000 and 3002;";
    std::string sql = "update aaa.file set resolution='nil' where id=3000;";
    std::string conn = "conn1";

    ckb::MyDBUtil::query_db(sql, conn);

    return 0;
}













