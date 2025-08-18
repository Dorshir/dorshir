#include "lexer.hpp"
#include "exceptions.hpp"

namespace rpn {

Lexer::Lexer(std::string const& expr, char separator)
: m_tokens{}
, m_next_to_pick{0}
{
    tokenize(expr, separator);
}


std::string_view Lexer::next()
{
    if (m_next_to_pick < m_tokens.size()) {
        return m_tokens[m_next_to_pick++];
    }
    throw OutOfRange();
}


void Lexer::tokenize(std::string const& expr, char separator)
{
    std::string_view view{expr};
    size_t start = 0;

    while (start < view.size()) {
        size_t end = view.find(separator, start);
        if (end == std::string_view::npos) {
            end = view.size();
        }
        if (end > start) {
            auto token = view.substr(start, end - start);
            if (!token.empty()) {
                m_tokens.emplace_back(token);
            }
        }
        start = end + 1;
    }

    if (start < view.size()) {
        m_tokens.emplace_back(view.substr(start));
    }
}
    

size_t Lexer::size() const
{
    return m_tokens.size();
}




} // namespace rpn