#ifndef BF_IO_HPP
#define BF_IO_HPP

#include <iostream> // for istream, ostream, cin, cout
#include <string>
#include <stddef.h> // for size_t

namespace bf {

class IO {

public:
    
    explicit IO(std::istream& in = std::cin, std::ostream& out = std::cout);
    ~IO() = default;
    IO(const IO&) = default;
    IO& operator=(const IO&) = default;

public:

    char read_buffered(); // const
    char read_stream(); // const
    void write_buffered(char c);
    void write_stream(char c);
    void set_input_buffer(const std::string& buffer);
    std::string get_output_buffer() const;
    void clear_output_buffer();

private:

    std::istream* m_in; // pointer or ref?
    std::ostream* m_out;
    std::string m_input_buffer;
    std::string m_output_buffer;
    size_t m_input_pos;
};



} // namespace bf

#endif // BF_IO_HPP
