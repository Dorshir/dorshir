#include "vigenère_decryptor.hpp"
#include <cstddef> // for size_t
#include <string>
#include <algorithm>


namespace palantir {

VigenereDecryptor::VigenereDecryptor(std::string const& keyword)
: m_keyword{keyword} 
{}

std::string VigenereDecryptor::encode(std::string const& message) const
{
    std::string result = message;
    size_t key_len = m_keyword.size();
    size_t j = 0;

    std::transform(message.begin(), message.end(), result.begin(), [&](char c) {
        if (c >= 'a' && c <= 'z') {
            char decoded = static_cast<char>((c - 'a' - (m_keyword[j % key_len] - 'a') + 26) % 26 + 'a');
            j++;
            return decoded;
        } else if (c >= 'A' && c <= 'Z') {
            char decoded = static_cast<char>((c - 'A' - (m_keyword[j % key_len] - 'A') + 26) % 26 + 'A');
            j++;
            return decoded;
        }
        return c;
    });

    return result;
}

} // namespace palantir
