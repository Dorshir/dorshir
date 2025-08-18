#ifndef PALANTIR_CONSOLE_SOURCE_HPP
#define PALANTIR_CONSOLE_SOURCE_HPP

#include "source.hpp"
#include <string>
#include <iostream>

namespace palantir {

class ConsoleSource : public Source {

public:

    explicit ConsoleSource(std::istream& input_stream = std::cin);
    ConsoleSource(ConsoleSource const&) = delete;
    ConsoleSource& operator=(ConsoleSource const&) = delete;
    ~ConsoleSource() = default;
    
    std::string read_message() override;
    bool has_more_messages() const override;

private:

    std::istream& m_input_stream;
    bool m_finished;

};

} // namespace palantir


#endif // PALANTIR_CONSOLE_SOURCE_HPP