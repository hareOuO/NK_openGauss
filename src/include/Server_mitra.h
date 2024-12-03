#pragma once
#include <string>
#include <map>
#include <vector>
#include "../common/interfaces/libpq/Mitra_common.h"


typedef uint64_t index_type;

using namespace std;


class MitraServer {
private:

    bool deleteFiles;
    bool useRocksDB;

public:
    map<prf_type, prf_type > DictW;
    MitraServer(bool useHDD,bool deleteFiles);
    void update(prf_type addr, prf_type val);
    vector<prf_type> search(vector<prf_type> KList);
    virtual ~MitraServer();

};
