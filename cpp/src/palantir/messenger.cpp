#include "messenger.hpp"
#include <string>

namespace palantir {

Messenger::Messenger(Source& source, Destination& destination, Encoder& encoder)
: m_source{source}
, m_dest{destination} 
, m_encoder{encoder}
{}

void Messenger::execute() 
{
    while (m_source.has_more_messages()) {
        std::string message = m_source.read_message();
        if (!message.empty()) {
            std::string encrypted_message = m_encoder.encode(message);
            m_dest.send_message(encrypted_message);
        }
    }
}

} // namespace palantir
