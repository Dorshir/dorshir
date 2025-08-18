#ifndef DP_TOKEN_HPP
#define DP_TOKEN_HPP

namespace dp {

/**
* @brief A restricted access token that grants special permissions to a specific class.
* 
* @tparam T The class that is granted access to this token.
*/
template<typename T>
class Token {

public:

    friend T; // Grants `T` access to private members. (e.g. constructors)

private:

    Token() = default;
    ~Token() = default;
    Token(Token const&) = default; 
    Token& operator=(Token const&) = default; 
    Token(Token&&) noexcept = default;
    Token& operator=(Token&&) noexcept = default;  

};



} // namespace dp


#endif // DP_TOKEN_HPP