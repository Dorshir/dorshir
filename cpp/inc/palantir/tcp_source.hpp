#ifndef PALANTIR_TCP_SOURCE_HPP
#define PALANTIR_TCP_SOURCE_HPP

#include "source.hpp"
#include "socket.hpp"
#include "address.hpp"

#include <string>

namespace palantir {

class TcpSource : public Source {

public:

    explicit TcpSource(Address const& local_address);
    TcpSource(TcpSource const&) = delete;
    TcpSource& operator=(TcpSource const&) = delete;
    ~TcpSource() = default;

    std::string read_message() override;
    bool has_more_messages() const override;

private:

    Socket m_server_socket;
    Socket m_client_socket;
    bool m_finished;

};

} // namespace palantir

#endif // PALANTIR_TCP_SOURCE_HPP
