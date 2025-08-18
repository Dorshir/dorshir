#include "file_dest.hpp"
#include <fstream>
#include <stdexcept>


namespace palantir {

FileDest::FileDest(std::string const& file_name)
: m_file{file_name}
{
    if (!m_file.is_open()) {
        throw std::runtime_error("Failed to open file: " + file_name);
    }
}

void FileDest::send_message(std::string const& message)
{
    m_file << message << std::endl;
    if (!m_file) {
        throw std::runtime_error("Failed to write to file.");
    }
}

} // namespace palantir