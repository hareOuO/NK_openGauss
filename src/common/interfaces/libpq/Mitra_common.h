#pragma once
#include <array>
#include <cryptopp/rng.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>

using namespace std;

#ifndef AES_KEY_SIZE
#define AES_KEY_SIZE CryptoPP::AES::DEFAULT_KEYLENGTH
typedef array<uint8_t, AES_KEY_SIZE> prf_type;
#endif


static inline bool is_base64(unsigned char c);
std::string base64_encode(const std::string &data);
std::string base64_decode(std::string const& encoded_string);