#ifndef PALANTIR_ENCODER_HPP
#define PALANTIR_ENCODER_HPP

#include <string>

namespace palantir {

class Encoder {

public:

    virtual ~Encoder() = default;
    virtual std::string encode(std::string const& message) const = 0;

protected:

    explicit Encoder() = default;
    Encoder(Encoder const&) = default;
    Encoder& operator=(Encoder const&) = default;

};


} // namespace palantir


#endif // PALANTIR_ENCODER_HPP