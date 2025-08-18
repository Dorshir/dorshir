#include "tcp_dest.hpp"
#include "address.hpp"
#include "socket.hpp"

#include <cstring>
#include <stdexcept>
#include <cstddef> 
#include <algorithm>

namespace palantir {

TcpDest::TcpDest(Address const& address)
: m_socket{address, Protocol::STREAM}
, m_connected(false)
{
    m_socket.connect_socket();
    m_connected = true;
}


void TcpDest::send_message(std::string const& message)
{
    if (!m_connected) {
        throw std::runtime_error("Socket is not connected");
    }
    std::vector<std::byte> buffer;
    buffer.reserve(message.size());

    std::transform(message.begin(), message.end(), std::back_inserter(buffer),
                   [](char c) -> std::byte { return static_cast<std::byte>(c); });
                   
    m_socket.send_data(buffer);
}

} // namespace palantir
