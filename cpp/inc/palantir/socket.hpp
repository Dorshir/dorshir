#ifndef PALANTIR_SOCKET_HPP
#define PALANTIR_SOCKET_HPP

#include "address.hpp"
#include "protocol.hpp"

#include <vector>
#include <cstddef>


namespace palantir {

class Socket {

public:

    explicit Socket(Address const& addr, Protocol protocol);
    explicit Socket(int socket_fd, Protocol protocol);
    ~Socket();
    Socket(Socket const&) = delete;
    Socket& operator=(Socket const&) = delete;

    void set_reuse_address(bool enable = true);
    void bind_socket();
    void connect_socket();
    void listen_socket(int backlog = 1);
    Socket accept_connection();

    ssize_t send_data(std::vector<std::byte> const& buffer, int flags = 0);
    ssize_t send_to(std::vector<std::byte> const& buffer, int flags = 0);
    ssize_t receive_data(std::vector<std::byte> & buffer, int flags = 0);
    ssize_t receive_from(std::vector<std::byte> & buffer, int flags = 0);

    int get_socket_fd() const;

private:

    Address m_address;
    Protocol m_protocol;
    int m_socket_fd;

    void close_socket();
};

} // namespace palantir

#endif // PALANTIR_SOCKET_HPP
