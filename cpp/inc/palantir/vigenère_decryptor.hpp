#ifndef PALANTIR_VIGENERE_DECRYPTOR_HPP
#define PALANTIR_VIGENERE_DECRYPTOR_HPP

#include "encoder.hpp"
#include <string>

namespace palantir {

class VigenereDecryptor : public Encoder {

public:

    explicit VigenereDecryptor(std::string const& keyword);
    VigenereDecryptor(VigenereDecryptor const&) = default;
    VigenereDecryptor& operator=(VigenereDecryptor const&) = default;
    ~VigenereDecryptor() = default;

    std::string encode(std::string const& message) const override;

private:

    std::string m_keyword;
    
};

} // namespace palantir

#endif // PALANTIR_VIGENERE_DECRYPTOR_HPP
