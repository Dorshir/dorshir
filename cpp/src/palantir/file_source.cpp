#include "file_source.hpp"
#include <stdexcept>

namespace palantir {

FileSource::FileSource(std::string const& filename)
: m_finished{false}, m_file{filename}
{
    if (!m_file || !m_file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
}

std::string FileSource::read_message()
{
    if (m_finished) {
        throw std::runtime_error("Read after reached eof");
    }

    std::string line;
    if (std::getline(m_file, line)) {
        return line;
    }

    m_finished = true;
    throw std::runtime_error("eof");
}

bool FileSource::has_more_messages() const
{
    return !m_finished;
}

} // namespace palantir
