#ifndef PALANTIR_VIGENERE_ENCRYPTOR_HPP
#define PALANTIR_VIGENERE_ENCRYPTOR_HPP

#include "encoder.hpp"
#include <string>

namespace palantir {

class VigenereEncryptor : public Encoder {

public:

    explicit VigenereEncryptor(std::string const& keyword);
    VigenereEncryptor(VigenereEncryptor const&) = default;
    VigenereEncryptor& operator=(VigenereEncryptor const&) = default;
    ~VigenereEncryptor() = default;

    std::string encode(std::string const& message) const override;

private:

    std::string m_keyword;

};

} // namespace palantir

#endif // PALANTIR_VIGENERE_ENCRYPTOR_HPP
