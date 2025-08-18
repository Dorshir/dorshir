#include "console_dest.hpp"
#include <string>
#include <iostream>

namespace palantir {
    

ConsoleDest::ConsoleDest(std::ostream &output_stream)
: m_output_stream{output_stream}
{}

void ConsoleDest::send_message(std::string const &message)
{
    m_output_stream << message << '\n';
}

} // namespace palantir


