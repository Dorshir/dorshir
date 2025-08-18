#ifndef PALANTIR_CAESAR_ENCODER_HPP
#define PALANTIR_CAESAR_ENCODER_HPP

#include "encoder.hpp"
#include <string>

namespace palantir {

class CaesarEncoder : public Encoder {

public:

    explicit CaesarEncoder(int shift);
    CaesarEncoder(CaesarEncoder const&) = default;
    CaesarEncoder& operator=(CaesarEncoder const&) = default;
    ~CaesarEncoder() = default;
    
    std::string encode(std::string const& message) const override;

private:

    int m_shift;

};


} // namespace palantir


#endif // PALANTIR_CAESAR_ENCODER_HPP