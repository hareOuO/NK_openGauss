#pragma once
#include<map>
#include<algorithm>
#include<string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include "../../sse/opensse-schemes/third_party/crypto/src/include/sse/crypto/hash.hpp"
#include "../../sse/opensse-schemes/third_party/crypto/src/include/sse/crypto/tdp.hpp"
#include "../../sse/opensse-schemes/lib/include/sse/schemes/sophos/sophos_common.hpp"

// using namespace std;
namespace sse {
namespace sophos {

extern std::string storage_dir_path  ;

extern const char* kTdpSkFile  ;
extern const char* kDerivationKeyFile ;
extern const char* kRsaPrgKeyFile ;

static constexpr size_t kMessageSize = 256;
// constexpr size_t kSearchTokenSize = kMessageSize;
static constexpr size_t kKeywordIndexSize = 16;

// constexpr size_t kSearchTokenSize   = crypto::Tdp::kMessageSize;
// constexpr size_t kDerivationKeySize = 32;
// constexpr size_t kUpdateTokenSize   = 16;

// using search_token_type = std::array<uint8_t, kSearchTokenSize>;
// using update_token_type = std::array<uint8_t, kUpdateTokenSize>;
// using index_type        = uint64_t;




struct new_UpdateRequest
{
    /*update_token_type token;
    index_type        index;*/
    std::string token;
    std::string index;
};

struct new_SearchRequest
{
    std::string token;
    std::string derivation_key;
    uint32_t add_count;
};


// 过滤函数
void filterRecords(std::vector<std::string> &records, int& count);


// 将字典内容覆盖写入文件
void writeDictionary(const std::map<std::string, int>& dictionary, const std::string& filename);

// 向文件中追加写入一条字典记录
void appendDictionaryEntry(const std::string& key, int value, const std::string& filename);

// 从文件中读取并恢复字典内容
std::map<std::string, int> loadDictionary(const std::string& filename);


class SophosClient
{
public:
    static constexpr size_t kKeywordIndexSize = 16;
    static constexpr size_t kKeySize          = 32;
	// SophosClient();
	SophosClient(const std::string& tdp_private_key, crypto::Key<kKeySize>&& derivation_master_key, crypto::Key<kKeySize>&& rsa_prg_key);
	~SophosClient();
	new_UpdateRequest insertion_request(std::string keyword, std::string index);
	new_UpdateRequest insertion_request(std::string keyword, std::string index, bool update_file);
	new_SearchRequest search_request(const std::string& keyword);

	std::string private_key() const;
    std::string public_key() const;

	std::string get_keyword_index(const std::string& kw);
	void get_and_increment(std::map<std::string, int>& keymap, std::string keyword, int& kw_counter);
	void get_and_increment(std::map<std::string, int>& keymap, std::string keyword, int& kw_counter, bool update_file);
	const sse::crypto::TdpInverse& inverse_tdp() const;
	const sse::crypto::Prf<kDerivationKeySize>& derivation_prf() const;
	std::map<std::string, int> counter_map;
private:
	// std::map<std::string, int> counter_map;
	sse::crypto::Prf<kDerivationKeySize> k_prf_;
	sse::crypto::TdpInverse inverse_tdp_;
	sse::crypto::Prf<sse::crypto::Tdp::kRSAPrfSize> rsa_prg_;
	std::string key = "mysecretkey";
};

void init_SophosClient_key();

SophosClient* init_SophosClient();

std::string Client_temphash(const std::string& str, const std::string& key);

std::string cyclicShift(const std::string& str, int shift) ;

//string back_cyclicShift(const string& str, int shift) {
//    int len = str.length();
//    shift = shift % len;  // ������λ�������ַ������ȵ����
//    std::string shiftedStr = str.substr(str.length() - shift) + str.substr(0, str.length() - shift);
//    return shiftedStr;
//}

std::string forward_prg(const std::string& st, uint32_t kw_counter);

std::string forward_prg(const std::string& seed);



std::string Client_xorStrings(const std::string& str1, const std::string& str2) ;


}//sophos
}//sse

extern sse::sophos::SophosClient* Sophos_client;