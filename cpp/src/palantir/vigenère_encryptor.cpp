#include "vigenère_encryptor.hpp"
#include <cstddef> // for size_t
#include <string>
#include <algorithm>


namespace palantir {

VigenereEncryptor::VigenereEncryptor(std::string const& keyword)
: m_keyword{keyword} 
{}

std::string VigenereEncryptor::encode(std::string const& message) const
{
    std::string result = message;
    size_t key_len = m_keyword.size();
    size_t j = 0;

    std::transform(message.begin(), message.end(), result.begin(), [&](char c) -> char {
        if (c >= 'a' && c <= 'z') {
            char encoded = static_cast<char>((c - 'a' + (m_keyword[j % key_len] - 'a')) % 26 + 'a');
            j++;
            return encoded;
        } else if (c >= 'A' && c <= 'Z') {
            char encoded = static_cast<char>((c - 'A' + (m_keyword[j % key_len] - 'A')) % 26 + 'A');
            j++;
            return encoded;
        }
        return c;
    });
    return result;
}

} // namespace palantir
