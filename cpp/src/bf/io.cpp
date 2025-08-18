#include "io.hpp"

namespace bf {

IO::IO(std::istream& in, std::ostream& out)
    : m_in{&in}
    , m_out{&out}
    , m_input_pos{0}
    {}


char IO::read_buffered()
{
    if (m_input_pos < m_input_buffer.size()) {
        return m_input_buffer[m_input_pos++];
    }
    return '\0';
}


char IO::read_stream()
{
    char c = '\0';
    m_in->get(c);
    return c;
}


void IO::write_buffered(char c)
{
    m_output_buffer.push_back(c);
}


void IO::write_stream(char c)
{
    m_out->put(c);
}



void IO::set_input_buffer(const std::string& buffer)
{
    m_input_buffer = buffer;
    m_input_pos = 0;
}


std::string IO::get_output_buffer() const
{
    return m_output_buffer;
}


void IO::clear_output_buffer()
{
    m_output_buffer.clear();
}

} // namespace bf
