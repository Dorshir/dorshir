#include "composite_encoder.hpp"
#include <string>
#include <vector>
#include <functional>


namespace palantir {


void CompositeEncoder::add(Encoder& enc)
{
    m_encoders.push_back(std::ref(enc));
}


std::string CompositeEncoder::encode(std::string const& message) const
{
    std::string result = message;
    for (auto& elem : m_encoders) {
        result = elem.get().encode(result);
    }
    return result;
}

} // namespace palantir
