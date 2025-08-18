#ifndef PALANTIR_ROT13_ENCODER_HPP
#define PALANTIR_ROT13_ENCODER_HPP

#include "encoder.hpp"
#include "caesar_encoder.hpp"
#include <string>

namespace palantir {

class Rot13Encoder : public Encoder {

public:

    explicit Rot13Encoder();
    Rot13Encoder(Rot13Encoder const&) = default;
    Rot13Encoder& operator=(Rot13Encoder const&) = default;
    ~Rot13Encoder() = default;
    
    std::string encode(std::string const& message) const override;

private:

    CaesarEncoder m_caesar;

};


} // namespace palantir


#endif // PALANTIR_ROT13_ENCODER_HPP