#include "atbash_encoder.hpp"
#include <string>
#include <algorithm>


namespace palantir {


std::string AtbashEncoder::encode(std::string const& message) const
{
    std::string result = message;
    std::transform(result.begin(), result.end(), result.begin(), [](char c) -> char {
        if (c >= 'a' && c <= 'z') {
            return 'z' - (c - 'a');
        } else if (c >= 'A' && c <= 'Z') {
            return 'Z' - (c - 'A');
        }
        return c;
        });
    return result;
}


}