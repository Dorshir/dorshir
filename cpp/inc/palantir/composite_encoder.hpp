#ifndef PALANTIR_COMPOSITE_ENCODER_HPP
#define PALANTIR_COMPOSITE_ENCODER_HPP

#include "encoder.hpp"
#include <string>
#include <vector>
#include <functional>

namespace palantir {

class CompositeEncoder : public Encoder {

public:

    explicit CompositeEncoder() = default;
    CompositeEncoder(CompositeEncoder const&) = default;
    CompositeEncoder& operator=(CompositeEncoder const&) = default;
    ~CompositeEncoder() = default;
    
    std::string encode(std::string const& message) const override;

    void add(Encoder& enc);

private:

    std::vector<std::reference_wrapper<Encoder>> m_encoders;

};


} // namespace palantir


#endif // PALANTIR_COMPOSITE_ENCODER_HPP