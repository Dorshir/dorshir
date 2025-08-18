#ifndef PALANTIR_CONSOLE_DEST_HPP
#define PALANTIR_CONSOLE_DEST_HPP

#include "destination.hpp"
#include <string>
#include <iostream>

namespace palantir {

class ConsoleDest : public Destination {

public:

    explicit ConsoleDest(std::ostream& output_stream = std::cout);
    ConsoleDest(ConsoleDest const&) = delete;
    ConsoleDest& operator=(ConsoleDest const&) = delete;
    ~ConsoleDest() = default;
    
    void send_message(std::string const& message) override;

private:

    std::ostream& m_output_stream;
};

} // namespace palantir


#endif // PALANTIR_CONSOLE_DEST_HPP