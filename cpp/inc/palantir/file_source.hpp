#ifndef PALANTIR_FILE_SOURCE_HPP
#define PALANTIR_FILE_SOURCE_HPP

#include "source.hpp"
#include <string>
#include <fstream>

namespace palantir {

class FileSource : public Source {

public:

    explicit FileSource(std::string const& filename);
    FileSource(FileSource const&) = delete;
    FileSource& operator=(FileSource const&) = delete;
    ~FileSource() = default;

    std::string read_message() override;
    bool has_more_messages() const override;

private:

    bool m_finished;
    std::ifstream m_file;

};

} // namespace palantir


#endif // PALANTIR_FILE_SOURCE_HPP