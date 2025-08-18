#ifndef PALANTIR_MESSENGER_HPP
#define PALANTIR_MESSENGER_HPP

#include "destination.hpp"
#include "source.hpp"
#include "encoder.hpp"

namespace palantir {

class Messenger 
{

public:

    explicit Messenger(Source& source, Destination& destination, Encoder& encoder);
    Messenger(Messenger const&) = delete;
    Messenger& operator=(Messenger const&) = delete;
    ~Messenger() = default;

    void execute();

private:

    Source& m_source;
    Destination& m_dest;
    Encoder& m_encoder;
    
};

} // namespace palantir



#endif // PLANTIR_MESSENGER_HPP