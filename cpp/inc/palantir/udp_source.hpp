#ifndef PALANTIR_UDP_SOURCE_HPP
#define PALANTIR_UDP_SOURCE_HPP

#include "source.hpp"
#include "socket.hpp"
#include "address.hpp"

#include <string>

namespace palantir {

class UdpSource : public Source {

public:

    explicit UdpSource(Address const& address);
    UdpSource(UdpSource const&) = delete;
    UdpSource& operator=(UdpSource const&) = delete;
    ~UdpSource() = default;

    std::string read_message() override;
    bool has_more_messages() const override;

private:

    Socket m_socket;
    bool m_finished;

};

} // namespace palantir

#endif // PALANTIR_UDP_SOURCE_HPP
