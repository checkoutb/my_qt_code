#include "ckb_myfileutil.h"

#include <QDebug>

#include <filesystem>
// #include <iostream>

namespace {

void dfsFile(std::filesystem::path fpath, std::vector<ckb::MyFileInfo>& vs) {
    for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(fpath)) {
        if (entry.is_directory()) {
            dfsFile(entry, vs);
        } else if (entry.is_regular_file()){
            vs.push_back(ckb::MyFileInfo(entry.path(), entry.path().filename(), entry.file_size() >> 20));
        } else {
            // block file
            // symlink
            // ...
        }
    }
}

}

namespace ckb {

std::vector<MyFileInfo> MyFileUtil::getAllFile(std::string dir_path) {
    std::vector<MyFileInfo> vf;
    dfsFile(dir_path, vf);
    std::sort(std::begin(vf), std::end(vf), [](const ckb::MyFileInfo& f1, const ckb::MyFileInfo& f2){
        return f1.name < f2.name;
    });
    return vf;
}

bool MyFileUtil::deleteAll(std::vector<std::string>& paths) {

    for (std::string& path : paths) {
        std::filesystem::remove(path);
    }
    return true;
}

}
