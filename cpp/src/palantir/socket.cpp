#include "socket.hpp"
#include "protocol.hpp"

#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>


namespace palantir {

Socket::Socket(Address const& address, Protocol protocol)
: m_address{address}
, m_protocol(protocol)
, m_socket_fd{::socket(AF_INET, static_cast<int>(protocol), 0)}
{
    if(m_socket_fd < 0) {
        throw std::runtime_error("Failed to create a socket");
    }
}


Socket::Socket(int socket_fd, Protocol protocol)
: m_address("", 0)
, m_protocol(protocol)
, m_socket_fd(socket_fd)
{
    if (m_socket_fd < 0) {
        throw std::runtime_error("Invalid socket file descriptor");
    }
}


Socket::~Socket()
{
    close_socket();
}


void Socket::bind_socket()
{
    auto* addr_ptr = m_address.get_sockaddr_ptr();
    auto  addr_len = m_address.get_sockaddr_size();

    if (::bind(m_socket_fd, addr_ptr, addr_len) < 0) {
        throw std::runtime_error("Failed to bind socket");
    }
}


void Socket::set_reuse_address(bool enable)
{
    int optval = enable ? 1 : 0;
    if (::setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        throw std::runtime_error("Failed to set SO_REUSEADDR");
    }
}


void Socket::connect_socket()
{
    auto* addr_ptr = m_address.get_sockaddr_ptr();
    auto  addr_len = m_address.get_sockaddr_size();
    if (::connect(m_socket_fd, addr_ptr, addr_len) < 0) {
        throw std::runtime_error("Failed to connect socket");
    }
}


void Socket::listen_socket(int backlog)
{
    if (::listen(m_socket_fd, backlog) < 0) {
        throw std::runtime_error("Failed to listen on socket");
    }
}


Socket Socket::accept_connection()
{
    sockaddr client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);

    int client_fd = ::accept(m_socket_fd, &client_addr, &client_addr_len);
    if (client_fd < 0) {
        throw std::runtime_error("Failed to accept connection");
    }

    return Socket(client_fd, m_protocol);
}


ssize_t Socket::send_data(std::vector<std::byte> const& buffer, int flags)
{
    ssize_t sent_bytes = ::send(m_socket_fd, buffer.data(), buffer.size(), flags);
    if(sent_bytes < 0) {
        throw std::runtime_error("send failed");
    }

    return sent_bytes;
}


ssize_t Socket::send_to(std::vector<std::byte> const& buffer, int flags)
{
    ssize_t sent_bytes = ::sendto(m_socket_fd, buffer.data(), buffer.size(), flags, m_address.get_sockaddr_ptr(), m_address.get_sockaddr_size());
    if(sent_bytes < 0) {
        throw std::runtime_error("send_to failed");
    }

    return sent_bytes;
}


ssize_t Socket::receive_data(std::vector<std::byte> & buffer, int flags)
{
    ssize_t ret = ::recv(m_socket_fd, buffer.data(), buffer.size(), flags);
    if(ret < 0) {
        throw std::runtime_error("recv failed");
    }

    return ret;
}


ssize_t Socket::receive_from(std::vector<std::byte> & buffer, int flags)
{
    sockaddr raw_src{};
    socklen_t raw_src_len = sizeof(raw_src);

    ssize_t ret = ::recvfrom(m_socket_fd, buffer.data(), buffer.size(), flags, &raw_src, &raw_src_len);
    if(ret < 0) {
        throw std::runtime_error("receive_from failed");
    }

    return ret;
}


int Socket::get_socket_fd() const
{
    return m_socket_fd;
}


void Socket::close_socket() 
{
    if (m_socket_fd >= 0) {
        ::close(m_socket_fd);
        m_socket_fd = -1;
    }
}


} // namespace palantir
