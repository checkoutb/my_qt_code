#ifndef CKB_MYDBUTIL_H
#define CKB_MYDBUTIL_H

#include <string>
#include <vector>
#include <utility>

// class MyDBUtil
// {
// public:
//     MyDBUtil();
// };

namespace ckb {


using MyDBResult = std::pair<std::vector<std::string>, std::vector<std::vector<std::string>>>;

struct MyDBUtil {

    static MyDBResult query_db(std::string& sql, std::string& db);

    static std::vector<std::string> get_all_conn_name();

    static const std::string sql_file;
};

}

#endif // CKB_MYDBUTIL_H
