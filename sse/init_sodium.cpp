#include <iostream>
#include "./opensse-schemes/third_party/crypto/src/include/sse/crypto/utils.hpp"

int main() {
    std::cout << "Initializing sodium library" << std::endl;
    sse::crypto::init_crypto_lib();
    return 0;
}
