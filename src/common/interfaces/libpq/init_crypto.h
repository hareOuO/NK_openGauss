#ifndef INIT_CRYPTO_H
#define INIT_CRYPTO_H

#include <iostream>
#include "../../../../sse/opensse-schemes/third_party/crypto/src/include/sse/crypto/utils.hpp"

class CryptoInitializer {
public:
    static void initialize() {
        static CryptoInitializer instance;
    }

private:
    CryptoInitializer() {
        std::cout << "Initializing crypto library" << std::endl;
        sse::crypto::init_crypto_lib();
    }
};

#endif // INIT_CRYPTO_H
