#ifndef PALANTIR_SOURCE_HPP
#define PALANTIR_SOURCE_HPP

#include <string>

namespace palantir {

class Source 
{

public:

    virtual ~Source() = default;

    virtual std::string read_message() = 0;
    virtual bool has_more_messages() const = 0;

protected:

    explicit Source() = default;
    Source(Source const&) = default;
    Source& operator=(Source const&) = default;

};

} // namespace palantir


#endif // PALANTIR_SOURCE_HPP