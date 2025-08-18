#ifndef PALANTIR_FILE_DEST_HPP
#define PALANTIR_FILE_DEST_HPP

#include "destination.hpp"
#include <string>
#include <fstream>

namespace palantir {

class FileDest : public Destination {

public:

    explicit FileDest(std::string const& file_name);
    FileDest(FileDest const&) = delete;
    FileDest& operator=(FileDest const&) = delete;
    ~FileDest() = default;

    void send_message(std::string const& message) override;

private:

    std::ofstream m_file;

};

} // namespace palantir


#endif // PALANTIR_FILE_DEST_HPP