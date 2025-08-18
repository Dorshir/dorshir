#include "udp_dest.hpp"
#include "address.hpp"
#include "protocol.hpp"

#include <cstring> 
#include <cstddef>
#include <algorithm>

namespace palantir {

UdpDest::UdpDest(Address const& address)
: m_socket{address, Protocol::DATAGRAM}
{}


void UdpDest::send_message(std::string const& message)
{
    std::vector<std::byte> buffer;
    buffer.reserve(message.size());

    std::transform(message.begin(), message.end(), std::back_inserter(buffer),
                   [](char c) -> std::byte { return static_cast<std::byte>(c); });

    m_socket.send_to(buffer);
}


void UdpDest::connect_socket()
{
    m_socket.connect_socket();
}

} // namespace palantir
