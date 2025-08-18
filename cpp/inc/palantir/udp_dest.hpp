#ifndef PALANTIR_UDP_DEST_HPP
#define PALANTIR_UDP_DEST_HPP

#include "destination.hpp"
#include "socket.hpp"
#include "address.hpp"

#include <string>

namespace palantir {

class UdpDest : public Destination {

public:

    explicit UdpDest(Address const& address);
    UdpDest(UdpDest const&) = delete;
    UdpDest& operator=(UdpDest const&) = delete;
    ~UdpDest() = default;

    void send_message(std::string const& message) override;

    void connect_socket();

private:

    Socket m_socket;

};

} // namespace palantir

#endif // PALANTIR_UDP_DEST_HPP
