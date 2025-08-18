#ifndef PALANTIR_TCP_DEST_HPP
#define PALANTIR_TCP_DEST_HPP

#include "destination.hpp"
#include "socket.hpp"
#include "address.hpp"
#include <string>

namespace palantir {

class TcpDest : public Destination {

public:

    explicit TcpDest(Address const& remote_address);
    TcpDest(TcpDest const&) = delete;
    TcpDest& operator=(TcpDest const&) = delete;
    ~TcpDest() = default;

    void send_message(std::string const& message) override;

private:
    
    Socket m_socket;
    bool m_connected;

};

} // namespace palantir

#endif // PALANTIR_TCP_DEST_HPP
