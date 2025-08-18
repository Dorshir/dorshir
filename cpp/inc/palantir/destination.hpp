#ifndef PALANTIR_DESTINATION_HPP
#define PALANTIR_DESTINATION_HPP

#include <string>

namespace palantir {

class Destination {

public:

    virtual ~Destination() = default;
    
    virtual void send_message(std::string const& message) = 0;

protected:

    explicit Destination() = default;
    Destination(Destination const&) = default;
    Destination& operator=(Destination const&) = default;

};

} // namespace palantir


#endif // PALANTIR_DESTINATION_HPP