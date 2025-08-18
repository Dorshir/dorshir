#ifndef RPN_LEXER_HPP
#define RPN_LEXER_HPP

#include <string>
#include <string_view>
#include <vector>

namespace rpn {

/**
* @brief A Lexer that tokenizes an input string into a sequence of tokens.
*
* The Lexer class splits an input string into tokens based on a specified separator.
* Tokens are stored internally as std::string_view objects, and can be retrieved one by one.
*/
class Lexer {

public:

    /**
    * @brief Constructs a new Lexer object.
    *
    * The constructor tokenizes the provided expression using whitespace as the default separator.
    *
    * @param expr The input expression to tokenize.
    * @param separator The character used to separate tokens.
    */
    Lexer(std::string const& expr, char separator = ' ');

    ~Lexer() = default;
    Lexer(const Lexer&) = default;
    Lexer& operator=(const Lexer&) = default;
    Lexer(Lexer&&) noexcept = default;
    Lexer& operator=(Lexer&&) noexcept = default;

public:

    /**
    * @brief Retrieves the next token from the tokenized input.
    *
    * @return std::string_view The next token in the sequence.
    *
    * @throws OutOfRange If there are no more tokens available.
    */
    std::string_view next();

    /**
    * @brief Gets the total number of tokens.
    *
    * @return size_t The total number of tokens available in the input.
    */
    size_t size() const;

private:

    /**
    * @brief Tokenizes the input expression into tokens.
    *
    * Splits the input expression into tokens using the given separator character and
    * stores the tokens in the internal vector.
    *
    * @param expr The input expression to tokenize.
    * @param separator The character used to separate tokens.
    */
    void tokenize(std::string const& expr, char separator);

private:

    std::vector<std::string_view> m_tokens;
    size_t m_next_to_pick;
};

} // namespace rpn

#endif // RPN_LEXER_HPP