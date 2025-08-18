#include "tcp_source.hpp"
#include "address.hpp"
#include "protocol.hpp"

#include <cstring>
#include <stdexcept>
#include <cstddef>
#include <vector>

namespace palantir {

TcpSource::TcpSource(Address const& local_address)
: m_server_socket(local_address, Protocol::STREAM)
, m_client_socket(m_server_socket.accept_connection())
, m_finished(false)
{
    m_server_socket.bind_socket();
    m_server_socket.listen_socket();
}


std::string TcpSource::read_message()
{
    if (m_finished) {
        return {};
    }

    std::vector<std::byte> buf(65536);

    ssize_t received = m_client_socket.receive_data(buf, 0);
    if (received == 0) {
        m_finished = true;
        return {};
    }

    std::string result;
    result.reserve(static_cast<size_t>(received));

    for (size_t i = 0; i < static_cast<size_t>(received); ++i) {
        result += static_cast<char>(buf[i]);
    }
    return result;

}


bool TcpSource::has_more_messages() const
{
    return !m_finished;
}

} // namespace palantir
