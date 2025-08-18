#include "udp_source.hpp"
#include "address.hpp"
#include "protocol.hpp"

#include <cstring>
#include <stdexcept>
#include <vector>
#include <cstddef> 

namespace palantir {

UdpSource::UdpSource(Address const& address)
: m_socket{address, Protocol::DATAGRAM}
{
    try {
        m_socket.bind_socket();
    } catch (std::runtime_error const& e) {
        throw std::runtime_error("UdpSource: Failed to bind socket");
    }
}


std::string UdpSource::read_message() 
{
    if (m_finished) {
        return {};
    }

    std::vector<std::byte> buffer(65536);

    try {
        ssize_t bytes_received = m_socket.receive_from(buffer);
        if (bytes_received == 0) {
            m_finished = true;
            return {};
        }
        std::string result;
        result.reserve(static_cast<size_t>(bytes_received));

        for (size_t i = 0; i < static_cast<size_t>(bytes_received); ++i) {
            result += static_cast<char>(buffer[i]);
        }
        return result;

    } catch (std::exception const&) {
        m_finished = true;
        return {};
    }
}


bool UdpSource::has_more_messages() const 
{
    return !m_finished;
}

} // namespace palantir
