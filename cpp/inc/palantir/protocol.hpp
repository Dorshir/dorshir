#ifndef PALANTIR_PROTOCOL_HPP
#define PALANTIR_PROTOCOL_HPP

#include <sys/socket.h>

namespace palantir {

enum class Protocol {
    DATAGRAM = SOCK_DGRAM,
    STREAM   = SOCK_STREAM
};

} // namespace palantir

#endif // PALANTIR_PROTOCOL_HPP
