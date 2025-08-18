#ifndef MT_TOKEN_HPP
#define MT_TOKEN_HPP

namespace mt {

/**
* @brief A restricted access token that grants special permissions to a specific class.
* 
* @tparam T The class that is granted access to this token.
*/
template<typename T>
class Token {

public:

    friend T; // Grants `T` access to private members. (e.g. constructors)

    ~Token() = default;

private:

    Token() = default;
    Token(Token const&) = delete; 
    Token& operator=(Token const&) = delete; 
    Token(Token&&) noexcept = delete;
    Token& operator=(Token&&) noexcept = delete;  

};



} // namespace mt


#endif // MT_TOKEN_HPP