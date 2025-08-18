#include "address.hpp"
#include <netinet/in.h>
#include <cstring>
#include <stdexcept>


namespace palantir {


Address::Address(std::string const &ip, int port)
: m_ip{ip}
, m_port{port}
{
    std::memset(&m_sockaddr, 0, sizeof(m_sockaddr));
    m_sockaddr.sin_family = AF_INET;
    m_sockaddr.sin_port = ::htons(static_cast<uint16_t>(port));
    m_sockaddr.sin_addr.s_addr = ::inet_addr(ip.c_str());
}




std::string const &Address::ip() const
{
    return m_ip;
}


int Address::port() const
{
    return m_port;
}


socklen_t Address::get_sockaddr_size() const
{
    return static_cast<socklen_t>(sizeof(m_sockaddr));
}


sockaddr const *Address::get_sockaddr_ptr() const
{
    return reinterpret_cast<sockaddr const*>(&m_sockaddr);
}


} // namespace palantir