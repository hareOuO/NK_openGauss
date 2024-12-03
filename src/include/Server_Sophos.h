#pragma once
#include<map>
#include<algorithm>
#include<string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <thread>
#include <unistd.h>
#include "../common/interfaces/libpq/init_crypto.h"
#include "../../sse/opensse-schemes/lib/include/sse/schemes/sophos/sophos_common.hpp"
#include "../../sse/opensse-schemes/third_party/crypto/src/include/sse/crypto/tdp.hpp"
#include "../../sse/opensse-schemes/third_party/crypto/src/include/sse/crypto/prf.hpp"

namespace sse{
namespace sophos{

extern const char* pk_file;
extern std::string storage_path_;
extern bool server_setup;

struct new_server_SearchRequest
{
    std::string token;
    std::string derivation_key;
    uint32_t add_count;
};

struct CryptoInitWrapper {
     CryptoInitWrapper() {
        CryptoInitializer::initialize();
    }
};


std::string Server_temphash(const std::string& str,const std::string& key);

std::string Server_xorStrings(const std::string& str1, const std::string& str2);

std::string back_cyclicShift(const std::string& str, int shift);

std::string back_prg(const std::string& st);


class SophosServer
{
public:
	// SophosServer();
	SophosServer(const std::string& tdp_pk);
    std::string public_key() const;
	~SophosServer();
	std::string search(new_server_SearchRequest sreq);
private:
	sse::crypto::TdpMultPool public_tdp_;
};

void init_SophosServer_key(std::string pk);

SophosServer* init_SophosServer();

}//sophos
}//sse
extern sse::sophos::SophosServer* Sophos_server;