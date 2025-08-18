#ifndef PALANTIR_ADDRESS_HPP
#define PALANTIR_ADDRESS_HPP

#include <string>
#include <arpa/inet.h>


namespace palantir {


class Address {

public:

    explicit Address(std::string const& ip, int port = 0);
    Address(Address const&) = default;
    Address& operator=(Address const&) = default;

    std::string const& ip() const;
    int port() const;
    socklen_t get_sockaddr_size() const;
    sockaddr const* get_sockaddr_ptr() const;

private:

    std::string m_ip;
    int m_port;
    sockaddr_in m_sockaddr;

};


} // namespace palantir


#endif // PALANTIR_ADDRESS_HPP
