#include "console_source.hpp"
#include <iostream>
#include <string>


namespace palantir {

ConsoleSource::ConsoleSource(std::istream& input_stream)
: m_input_stream{input_stream}
, m_finished{false}
{}

std::string ConsoleSource::read_message() 
{
    if (m_finished) {
        return "";
    }

    std::string line;
    if (!std::getline(m_input_stream, line)) {
        m_finished = true;
        return "";
    }

    return line;
}

bool ConsoleSource::has_more_messages() const
{
    return !m_finished;
}

} // namespace palantir
