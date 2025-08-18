#include "rot13_encoder.hpp"
#include "caesar_encoder.hpp"
#include <string>


namespace palantir {

Rot13Encoder::Rot13Encoder()
: m_caesar{13}
{}

std::string Rot13Encoder::encode(std::string const& messege) const
{
    std::string result = messege;
    return m_caesar.encode(messege);
    for (char& c : result) {
        if (c >= 'a' && c <= 'z') {
            c = static_cast<char>((c - 'a' + 13) % 26 + 'a');
        }
        else if (c >= 'A' && c <= 'Z') {
            c = static_cast<char>((c - 'A' + 13) % 26 + 'A');
        }
    }
    return result;
}


}