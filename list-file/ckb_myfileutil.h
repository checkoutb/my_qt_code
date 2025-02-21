#ifndef CKB_MYFILEUTIL_H
#define CKB_MYFILEUTIL_H

#include <vector>
#include <string>


namespace ckb {

struct MyFileInfo {
    std::string path;
    std::string name;
    int mb;

    MyFileInfo(std::string path, std::string name, int mb) : path(path), name(name), mb(mb) {}

};

struct MyFileUtil {
    static std::vector<MyFileInfo> getAllFile(std::string dir_path);
    static bool deleteAll(std::vector<std::string>& vPath);
};

}

#endif // CKB_MYFILEUTIL_H
