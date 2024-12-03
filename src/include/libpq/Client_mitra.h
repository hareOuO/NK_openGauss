#pragma once
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include "../../common/interfaces/libpq/Mitra_common.h"
using namespace std;

enum OP {
    INS, DEL
};

// #ifndef AES_KEY_SIZE
// // #define AES_KEY_SIZE CryptoPP::AES::DEFAULT_KEYLENGTH
// #define AES_KEY_SIZE 256
// typedef array<uint8_t, AES_KEY_SIZE> prf_type;
// #endif

extern string storage_dir_path;

// 将字典内容覆盖写入文件
void writeDictionary(const std::map<std::string, int>& dictionary, const std::string& filename);

// 向文件中追加写入一条字典记录
void appendDictionaryEntry(const std::string& key, int value, const std::string& filename);

// 从文件中读取并恢复字典内容
std::map<std::string, int> loadDictionary(const std::string& filename);

class MitraClient {
private:
    inline prf_type bitwiseXOR(int input1, int op, prf_type input2);
    inline prf_type bitwiseXOR(prf_type input1, prf_type input2);
    inline void getAESRandomValue(unsigned char* keyword, int op, int srcCnt, int counter, unsigned char* result);
    // Server* server;
    bool deleteFiles;
    double totalUpdateCommSize;
    double totalSearchCommSize;
    bool setupMode;
    map<string, int> FileCnt;
    map<string, int> SrcCnt;

public:
    // Client(Server* server, bool deleteFiles);
    MitraClient(bool deleteFiles);
    // void update(OP op, string keyword, int ind);
    // vector<int> search(string keyword);
    void updateRequest(OP op, string keyword, int ind, prf_type& address, prf_type& value);
    prf_type searchRequest(string keyword, vector<string>& tokens);
    void searchProcess(vector<prf_type> tokens, string keyword, vector<int>& ids, vector<prf_type>& addrs, vector<prf_type>& vals);
    virtual ~MitraClient();
    int getFileCntSize() const;
    bool isSetupMode() const;
    double getTotalSearchCommSize() const;
    double getTotalUpdateCommSize() const;
    void setSetupMode(bool setupMode);

};
extern MitraClient* Mitra_client;
