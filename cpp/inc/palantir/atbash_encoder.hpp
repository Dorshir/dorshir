#ifndef PALANTIR_ATBASH_ENCODER_HPP
#define PALANTIR_ATBASH_ENCODER_HPP

#include "encoder.hpp"
#include <string>

namespace palantir {

class AtbashEncoder : public Encoder {

public:

    explicit AtbashEncoder() = default;
    AtbashEncoder(AtbashEncoder const&) = default;
    AtbashEncoder& operator=(AtbashEncoder const&) = default;
    ~AtbashEncoder() = default;
    
    std::string encode(std::string const& message) const override;

};


} // namespace palantir


#endif // PALANTIR_ATBASH_ENCODER_HPP