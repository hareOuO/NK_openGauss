#include "libpq/Client_Sophos.h"
#include <iostream>
#include "../../../../sse/opensse-schemes/lib/include/sse/schemes/sophos/sophos_common.hpp"
namespace sse {
namespace sophos {

std::string storage_dir_path  = "/opt/og/openGauss-server/sse_storage";

const char* kTdpSkFile         = "tdp_sk.key";
const char* kDerivationKeyFile = "derivation_master.key";
const char* kRsaPrgKeyFile  = "rsa_prg.key";



// 过滤函数
void filterRecords(std::vector<std::string> &records, int& count) {
    std::unordered_map<int, bool> deletedIds;

    // 第一次遍历，记录所有被删除的 id
    for (int i = 0; i < count; ++i) {
        int id = std::stoi(records[i].substr(0, records[i].find('|')));
        std::string action = (records[i]).substr(records[i].find('|') + 1);
        if (action == "del") {
            deletedIds[id] = true;
        }
    }

    // 第二次遍历，筛选出未被删除的记录
    std::vector<std::string> filteredRecords;
    for (int i = 0; i < count; ++i) {
        int id = std::stoi(records[i].substr(0, records[i].find('|')));
        if (deletedIds.find(id) == deletedIds.end()) {
            filteredRecords.push_back(std::to_string(id));
        }
    }

    // 将过滤后的记录存回原始数组
    //delete[] * records; // 释放原始内存


    // 释放内存
    records.clear();
    count = filteredRecords.size(); // 更新记录数

    records.assign(filteredRecords.begin(),filteredRecords.end());

}

// 将字典内容覆盖写入文件
void writeDictionary(const std::map<std::string, int>& dictionary, const std::string& filename) {
	std::ofstream file(filename);
	if (file.is_open()) {
		for (const auto& pair : dictionary) {
			file << pair.first << " " << pair.second << std::endl;
		}
		file.close();
		//cout << "Dictionary written to file: " << filename << endl;
	}
	else {
		//cerr << "Error: Unable to open file for writing: " << filename << endl;
	}
}

// 向文件中追加写入一条字典记录
void appendDictionaryEntry(const std::string& key, int value, const std::string& filename) {
	std::ofstream file(filename,std::ios::app);
	if (file.is_open()) {
		file << key << " " << value << std::endl;
		file.close();
		//cout << "New entry appended to file: " << filename << endl;
	}
	else {
		//cerr << "Error: Unable to open file for writing: " << filename << endl;
	}
}

// 从文件中读取并恢复字典内容
std::map<std::string, int> loadDictionary(const std::string& filename) {
	std::map<std::string, int> dictionary;
	std::ifstream file(filename);
	// if (file.is_open()) {
	// 	std::string key;
	// 	int value;
	// 	while (file >> key >> value) {
	// 		dictionary[key] = value;
	// 	}
	// 	file.close();
	// 	std::cout << "Dictionary loaded from file: " << filename << std::endl;
	// }
	if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::size_t pos = line.find_last_of(' ');
            if (pos != std::string::npos) {
                std::string name = line.substr(0, pos);  // 提取名字部分
                int value = std::stoi(line.substr(pos + 1));  // 提取并转换最后的整数部分
                dictionary[name] = value;
            }
        }
        file.close();
        std::cout << "Dictionary loaded from file: " << filename << std::endl;
    }
	else {
		std::cout << "Error: Unable to open file for reading: " << filename << std::endl;
	}
	return dictionary;
}

std::string Client_temphash(const std::string& str, const std::string& key)
{
	unsigned int hash = 0;
	for (size_t i = 0; i < str.length(); ++i) {
		hash += (str[i] * key[i % key.length()]);
	}
	std::stringstream ss;
	ss << std::hex << hash;
	return ss.str();
}

std::string cyclicShift(const std::string& str, int shift) {
    int len = str.length();
    shift = shift % len;
    std::string shiftedStr = str.substr(shift) + str.substr(0, shift);
    return shiftedStr;
}

std::string forward_prg(const std::string& st, uint32_t kw_counter)
{
    return cyclicShift(st, kw_counter);
}

std::string forward_prg(const std::string& seed)
{
    return cyclicShift(seed, 1);
}

std::string Client_xorStrings(const std::string& str1, const std::string& str2) {
    std::string result;
    int a = str1.length();
    int b = str2.length();
    for (size_t i = 0; i < a && i < b; ++i)
    {
        result += str1[i % a] ^ str2[i % b];
    }
    return result;
}

SophosClient::SophosClient(const std::string& tdp_private_key, crypto::Key<kKeySize>&& derivation_master_key, crypto::Key<kKeySize>&& rsa_prg_key)
    : k_prf_(std::move(derivation_master_key)), inverse_tdp_(tdp_private_key),
      rsa_prg_(std::move(rsa_prg_key))
{
	counter_map = loadDictionary("/opt/og/openGauss-server/sse_storage/fides_client.mat");
}

SophosClient::~SophosClient()
{
}

// generate the keys
void init_SophosClient_key()
{
	std::string sk_path          = storage_dir_path + "/" + kTdpSkFile;
    std::string master_key_path  = storage_dir_path + "/" + kDerivationKeyFile;
    std::string rsa_prg_key_path = storage_dir_path + "/" + kRsaPrgKeyFile;

    std::array<uint8_t, SophosClient::kKeySize> derivation_master_key
        = crypto::random_bytes<uint8_t, SophosClient::kKeySize>();
    std::array<uint8_t, SophosClient::kKeySize> rsa_prg_key
        = crypto::random_bytes<uint8_t, SophosClient::kKeySize>();
    crypto::TdpInverse tdp;

	std::ofstream sk_out(sk_path.c_str());
    if (!sk_out.is_open()) {
        throw std::runtime_error(sk_path + ": unable to write the secret key");
    }
    sk_out << tdp.private_key();
    sk_out.close();

	std::ofstream master_key_out(master_key_path.c_str());
    if (!master_key_out.is_open()) {
        throw std::runtime_error(
            master_key_path + ": unable to write the master derivation key");
    }
    master_key_out << std::string(derivation_master_key.begin(),
                                  derivation_master_key.end());
    master_key_out.close();

    std::ofstream rsa_prg_key_out(rsa_prg_key_path.c_str());
    if (!rsa_prg_key_out.is_open()) {
        throw std::runtime_error(rsa_prg_key_path
                                 + ": unable to write the rsa prg key");
    }
    rsa_prg_key_out << std::string(rsa_prg_key.begin(), rsa_prg_key.end());
    rsa_prg_key_out.close();
}

SophosClient* init_SophosClient()
{
	// try to initialize everything from this directory
    std::string sk_path          = storage_dir_path + "/" + kTdpSkFile;
    std::string master_key_path  = storage_dir_path + "/" + kDerivationKeyFile;
    std::string rsa_prg_key_path = storage_dir_path + "/" + kRsaPrgKeyFile;

    std::ifstream     sk_in(sk_path.c_str());
    std::ifstream     master_key_in(master_key_path.c_str());
    std::ifstream     rsa_prg_key_in(rsa_prg_key_path.c_str());
    std::stringstream sk_buf;
    std::stringstream master_key_buf;
    std::stringstream rsa_prg_key_buf;

    sk_buf << sk_in.rdbuf();
    master_key_buf << master_key_in.rdbuf();
    rsa_prg_key_buf << rsa_prg_key_in.rdbuf();

	std::array<uint8_t, 32> client_master_key_array;
    std::array<uint8_t, 32> client_tdp_prg_key_array;

    if (master_key_buf.str().size() != client_master_key_array.size()) {
        throw std::runtime_error(
            "Invalid master key size when constructing the Sophos client: "
            + std::to_string(master_key_buf.str().size())
            + " bytes instead of 32");
    }
    if (rsa_prg_key_buf.str().size() != client_tdp_prg_key_array.size()) {
        throw std::runtime_error(
            "Invalid PRG key size when constructing the Sophos client: "
            + std::to_string(rsa_prg_key_buf.str().size())
            + " bytes instead of 32");
    }
    auto master_key_str  = master_key_buf.str();
    auto rsa_prg_key_str = rsa_prg_key_buf.str();

    std::copy(master_key_str.begin(),
              master_key_str.end(),
              client_master_key_array.begin());
    std::copy(rsa_prg_key_str.begin(),
              rsa_prg_key_str.end(),
              client_tdp_prg_key_array.begin());

	return new SophosClient(sk_buf.str(),
                         sse::crypto::Key<SophosClient::kKeySize>(
                             client_master_key_array.data()),
                         sse::crypto::Key<SophosClient::kKeySize>(
                             client_tdp_prg_key_array.data()));
}

std::string SophosClient::get_keyword_index(const std::string& kw)
{
	// std::string hash_string = Client_temphash(kw,key);
	std::string hash_string = sse::crypto::Hash::hash(kw);
	/*return hash_string.erase(kKeywordIndexSize);*/
	return hash_string;
}

void SophosClient::get_and_increment(std::map<std::string, int>& keymap, std::string keyword, int& kw_counter)
{
	auto it = keymap.find(keyword);
	if (it != keymap.end())
	{
		kw_counter = ++keymap[keyword];
		writeDictionary(keymap, "/opt/og/openGauss-server/sse_storage/fides_client.mat");
	}
	else
	{
		keymap.insert(make_pair(keyword, 1));
		appendDictionaryEntry(keyword,1, "/opt/og/openGauss-server/sse_storage/fides_client.mat");
		kw_counter = 1;
	}
}
void SophosClient::get_and_increment(std::map<std::string, int>& keymap, std::string keyword, int& kw_counter, bool update_file)
{
	auto it = keymap.find(keyword);
	if (it != keymap.end())
	{
		kw_counter = ++keymap[keyword];
		if(update_file)
		{
			writeDictionary(keymap, "/opt/og/openGauss-server/sse_storage/fides_client.mat");
		}
	}
	else
	{
		keymap.insert(make_pair(keyword, 1));
		appendDictionaryEntry(keyword,1, "/opt/og/openGauss-server/sse_storage/fides_client.mat");
		kw_counter = 1;
	}
}

std::string SophosClient::public_key() const
{
    return inverse_tdp_.public_key();
}

std::string SophosClient::private_key() const
{
    return inverse_tdp_.private_key();
}

const sse::crypto::TdpInverse& SophosClient::inverse_tdp() const
{
    return inverse_tdp_;
}

const sse::crypto::Prf<kDerivationKeySize>& SophosClient::derivation_prf() const
{
    return k_prf_;
}

new_UpdateRequest SophosClient::insertion_request(std::string keyword, std::string index)//
{
	sse::sophos::new_UpdateRequest req;
	// std::string st;
	search_token_type st;
	update_token_type pre_ut;
	std::string seed = get_keyword_index(keyword);
	int kw_counter = 0;
	get_and_increment(counter_map, keyword, kw_counter);

	// st = forward_prg(seed);
	st = inverse_tdp().generate_array(rsa_prg_, seed);//计算ST0

	if (kw_counter == 1) {
		//cout << "ST1" << endl;
	}
	else {
		// st = forward_prg(st, kw_counter - 1);
		st = inverse_tdp().invert_mult(st, kw_counter-1);//c≠1，则计算STc
	}
	
	// req.token = Client_temphash(st, key);//UT

	auto deriv_key = derivation_prf().prf(reinterpret_cast<const uint8_t*>(seed.data()), kKeywordIndexSize);//用来计算UT
	// std::string st_string(reinterpret_cast<const char*>(st.data()),kSearchTokenSize);

	std::array<uint8_t, kUpdateTokenSize> mask;
	gen_update_token_masks(crypto::Prf<kUpdateTokenSize>(crypto::Key<kKeySize>(deriv_key.data())),st.data(),pre_ut,mask);//计算UT

	req.token = base64_encode(std::string(pre_ut.begin(), pre_ut.end()));

    // req.token = crypto::Prf<kUpdateTokenSize>(crypto::Key<kKeySize>(deriv_key.data())).prf(st_string + '0');
	req.index = index;

	return req;
}
new_UpdateRequest SophosClient::insertion_request(std::string keyword, std::string index, bool update_file)//
{
	sse::sophos::new_UpdateRequest req;
	// std::string st;
	search_token_type st;
	update_token_type pre_ut;
	std::string seed = get_keyword_index(keyword);
	int kw_counter = 0;
	get_and_increment(counter_map, keyword, kw_counter, update_file);

	// st = forward_prg(seed);
	st = inverse_tdp().generate_array(rsa_prg_, seed);//计算ST0

	if (kw_counter == 1) {
		//cout << "ST1" << endl;
	}
	else {
		// st = forward_prg(st, kw_counter - 1);
		st = inverse_tdp().invert_mult(st, kw_counter-1);//c≠1，则计算STc
	}
	
	// req.token = Client_temphash(st, key);//UT

	auto deriv_key = derivation_prf().prf(reinterpret_cast<const uint8_t*>(seed.data()), kKeywordIndexSize);//用来计算UT
	// std::string st_string(reinterpret_cast<const char*>(st.data()),kSearchTokenSize);

	std::array<uint8_t, kUpdateTokenSize> mask;
	gen_update_token_masks(crypto::Prf<kUpdateTokenSize>(crypto::Key<kKeySize>(deriv_key.data())),st.data(),pre_ut,mask);//计算UT

	req.token = base64_encode(std::string(pre_ut.begin(), pre_ut.end()));

    // req.token = crypto::Prf<kUpdateTokenSize>(crypto::Key<kKeySize>(deriv_key.data())).prf(st_string + '0');
	req.index = index;

	return req;
}


new_SearchRequest SophosClient::search_request(const std::string& keyword)
{
	uint32_t      kw_counter = 0;
	bool          found;
	new_SearchRequest req;
	req.add_count = 0;
	search_token_type pre_st;

	std::string seed = get_keyword_index(keyword);
	auto iter = counter_map.find(keyword);
	if (iter != counter_map.end()) {
		found = true;
		kw_counter = iter->second;
	}
	else {
		found = false;
	}

	if (!found) {
		//cout << "No matching counter found for keyword ";
	}
	else {
		// Now derive the original search token from the kw_index (as seed)
		// req.token = forward_prg(seed);
		// req.token = forward_prg(req.token, kw_counter - 1);

		pre_st = inverse_tdp().generate_array(rsa_prg_, seed);            //ST0
        pre_st = inverse_tdp().invert_mult(pre_st, kw_counter-1);         //STc

		auto deriv_key=derivation_prf().prf(reinterpret_cast<const uint8_t*>(seed.data()), kKeywordIndexSize);;
		req.derivation_key = base64_encode(std::string(deriv_key.begin(), deriv_key.end()));
		req.add_count = kw_counter;

		req.token=base64_encode(std::string(pre_st.begin(), pre_st.end()));
	}

	return req;
}



}//sophos
}//sse
sse::sophos::SophosClient* Sophos_client = nullptr;