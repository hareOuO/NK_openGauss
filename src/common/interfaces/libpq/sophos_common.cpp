//
//  sophos_common.cpp
//  SSE_Schemes
//
//  Created by Raphael Bost on 04/10/2017.
//  Copyright © 2017 Raphael Bost. All rights reserved.
//

#include <../../../../sse/opensse-schemes/lib/include/sse/schemes/sophos/sophos_common.hpp>

#include <../../../../sse/opensse-schemes/third_party/crypto/src/include/sse/crypto/prf.hpp>

#include <cstring>

namespace sse {
namespace sophos {


static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

// std::string base64_encode(const std::string &data) {
//     static const std::string base64_chars = 
//              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//              "abcdefghijklmnopqrstuvwxyz"
//              "0123456789+/";
 
//     if (data.empty()) return "";
 
//     size_t data_len = data.size();
//     size_t data_len_plus_one = data_len + 1;
//     size_t mod_4 = data_len_plus_one % 4;
//     size_t output_length = data_len_plus_one + ((mod_4 != 0) ? (4 - mod_4) : 0);
//     std::string base64_str(output_length, '=');
 
//     for (size_t i = 0, j = 0; i < data_len;) {
//         unsigned char octet_a = i < data_len ? data[i] : 0;
//         unsigned char octet_b = i + 1 < data_len ? data[i + 1] : 0;
//         unsigned char octet_c = i + 2 < data_len ? data[i + 2] : 0;
 
//         unsigned char triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
 
//         base64_str[j++] = base64_chars[(triple >> 3 * 6) & 0x3F];
//         base64_str[j++] = base64_chars[(triple >> 2 * 6) & 0x3F];
//         base64_str[j++] = base64_chars[(triple >> 1 * 6) & 0x3F];
//         base64_str[j++] = base64_chars[(triple >> 0 * 6) & 0x3F];
 
//         i += 3;
//     }
 
//     return base64_str;
// }


std::string base64_encode(const std::string &in) {
    std::string out;
    int val = 0, valb = -6;
    for (uint8_t c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

std::string base64_decode(const std::string &encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    uint8_t char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}


void gen_update_token_masks(const crypto::Prf<kUpdateTokenSize>& derivation_prf,
                            const uint8_t*                       search_token,
                            update_token_type&                   update_token,
                            std::array<uint8_t, kUpdateTokenSize>& mask)
{
    //            auto derivation_prf =
    //            crypto::Prf<kUpdateTokenSize>(deriv_key);

    std::string st_string(reinterpret_cast<const char*>(search_token),
                          kSearchTokenSize);

    update_token = derivation_prf.prf(st_string + '0');
    mask         = derivation_prf.prf(st_string + '1');
}

} // namespace sophos
} // namespace sse
