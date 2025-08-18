#include "caesar_encoder.hpp"
#include <string>
#include <algorithm>


namespace palantir {

CaesarEncoder::CaesarEncoder(int shift)
: m_shift{shift}
{}

std::string CaesarEncoder::encode(std::string const& message) const
{
    std::string result = message;
    std::transform(result.begin(), result.end(), result.begin(), [this](char c) -> char {
        if (c >= 'a' && c <= 'z') {
            c = static_cast<char>((c - 'a' + m_shift + 26) % 26 + 'a');
        } else if (c >= 'A' && c <= 'Z') {
            c = static_cast<char>((c - 'A' + m_shift + 26) % 26 + 'A');
        }
        return c;
        });
    return result;
}


}