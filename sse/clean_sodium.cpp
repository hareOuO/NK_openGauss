#include <iostream>
#include "./opensse-schemes/third_party/crypto/src/include/sse/crypto/utils.hpp"

int main() {
    std::cout << "clean sodium library" << std::endl;
    sse::crypto::cleanup_crypto_lib();
    return 0;
}
