#include "Server_Sophos.h"
#include "libpq/Client_Sophos.h"

namespace sse{
namespace sophos{


const char* pk_file = "tdp_pk.key";
std::string storage_path_="/opt/og/openGauss-server/sse_server_storage";
bool server_setup=false;

// struct new_SearchRequest
// {
//     std::string token;
//     std::string derivation_key;
//     uint32_t add_count;
// };


std::string Server_temphash(const std::string& str,const std::string& key)
{
	unsigned int hash = 0;
	for (size_t i = 0; i < str.length(); ++i) {
		hash += (str[i] * key[i % key.length()]); 
	}
	std::stringstream ss;
	ss << std::hex << hash;
	return ss.str();
}

std::string Server_xorStrings(const std::string& str1, const std::string& str2) {
	std::string result;
	int a = str1.length();
	int b = str2.length();
	for (size_t i = 0; i < a && i < b; ++i)
	{
		result += str1[i % a] ^ str2[i % b];
	}
	return result;
}

std::string back_cyclicShift(const std::string& str, int shift) {
	int len = str.length();
	shift = shift % len;  
	std::string shiftedStr = str.substr(str.length() - shift) + str.substr(0, str.length() - shift);
	return shiftedStr;
}

std::string back_prg(const std::string& st)
{
	return back_cyclicShift(st, 1);
}



// SophosServer::SophosServer()
// {
// }

SophosServer::SophosServer(const std::string& tdp_pk) : public_tdp_(tdp_pk, 2 * std::thread::hardware_concurrency())
{
}

SophosServer::~SophosServer()
{
}

std::string SophosServer::public_key() const
{
    return public_tdp_.public_key();
}


void init_SophosServer_key(std::string pk)
{
    // write the public key in a file
    std::string pk_path = storage_path_ + "/" + pk_file;

    std::ofstream pk_out(pk_path.c_str());
    if (!pk_out.is_open()) {
        // error
        throw std::runtime_error(
        pk_path + ": unable to write the public key");

    }
	pk_out<<pk;
    // pk_out << Sophos_client->public_key();
    pk_out.close();

	std::ofstream outfile;
	outfile.open(storage_path_+"/server_log.txt", std::ios::app);
	std::time_t now = std::time(nullptr);
    char* dt = std::ctime(&now);
	pid_t pid = getpid();
    outfile << dt <<" [PID: " << pid << "] "<<"       server key ok"<<std::endl;
    outfile.close();
}

SophosServer* init_SophosServer()
{
    std::string pk_path = storage_path_ + "/" + pk_file;
    std::ifstream pk_in(pk_path.c_str());
    std::stringstream pk_buf;

    pk_buf << pk_in.rdbuf();

	std::ofstream outfile;
    outfile.open("/opt/og/openGauss-server/sse_server_storage/server_log.txt", std::ios::app);
    std::time_t now = std::time(nullptr);
    char* dt = std::ctime(&now);
    pid_t pid = getpid();
    outfile <<dt<<" [PID: " << pid << "] "<<"     Sophos_server  'init or re_init' ok"<<std::endl;
    outfile.close();

    return new SophosServer(pk_buf.str());
	// return new SophosServer();
}

std::string SophosServer::search(new_server_SearchRequest sreq)
{
	// std::string key = "mysecretkey";

	search_token_type st;
	std::copy(sreq.token.begin(), sreq.token.end(), st.begin());

    std::array<uint8_t, kDerivationKeySize> DerivationKey_arr = {0};
    std::copy(sreq.derivation_key.begin(), sreq.derivation_key.begin() + kDerivationKeySize, DerivationKey_arr.begin());

	crypto::Prf<kUpdateTokenSize> derivation_prf(crypto::Key<kDerivationKeySize>(DerivationKey_arr.data()));

	// std::string R = "DELETE FROM fides_indextable WHERE UT IN ('";
	std::string R = "DELETE FROM athletes_index WHERE UT IN ('";
	std::string tem="";
	for (int i = sreq.add_count; i > 0; i--)
	{
		update_token_type ut;
        std::array<uint8_t, kUpdateTokenSize> mask;
		gen_update_token_masks(derivation_prf, st.data(), ut, mask);// UT

		tem+=base64_encode(std::string(ut.begin(), ut.end()));
		//tem = xorStrings(Server_temphash(sreq.derivation_key,key), Server_temphash(sreq.token,key));
		// tem = Server_temphash(sreq.token, key);

		if (i != 1)
		{
			tem = tem + "','";
		}

		st = public_tdp_.eval(st);
	}
	R = R + tem + "') RETURNING *;";
	return R;
}
}//sophos
}//sse
sse::sophos::SophosServer* Sophos_server = nullptr;
