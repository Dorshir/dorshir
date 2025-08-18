#ifndef RPN_PARSER_HPP
#define RPN_PARSER_HPP

#include <memory>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "lexer.hpp"
#include "node.hpp"

namespace rpn {

/**
* @brief The Parser class builds an expression tree from tokens provided by a Lexer.
*
* This class implements a parser for expressions in Reverse Polish Notation (postfix notation).
* It consumes tokens from a Lexer and constructs an abstract syntax tree (AST).
*/
class Parser {

public:

    /**
    * @brief Constructs a Parser with a reference to a Lexer.
    *
    * @param lexer A reference to a Lexer instance that provides tokens from an input expression.
    */
    Parser(Lexer& lexer);
    
    ~Parser() = default;
    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;
    Parser(Parser&&) noexcept = default;
    Parser& operator=(Parser&&) noexcept = default;

public:

    /**
    * @brief Parses the tokens and builds an expression tree.
    *
    * @return A unique pointer to the root Node of the constructed expression tree.
    *
    * @throws InvalidExpression If the expression is invalid (e.g., due to mismatched operands and operators).
    */
    std::unique_ptr<Node> parse();

private:

    /**
    * @brief Determines if a token represents a number.
    *
    * @param tok The token to check.
    * @return true if the token is a number, false otherwise.
    */
    bool is_number(std::string_view tok);

    /**
    * @brief Determines if a token represents an operator.
    *
    * @param tok The token to check.
    * @return true if the token is one of the supported operators ("+", "-", "*", "/"), false otherwise.
    */
    bool is_operator(std::string_view tok);

    /**
    * @brief Initializes the supported binary operator functions.
    *
    * This method populates an internal map with operator symbols as keys and corresponding binary
    * operator functions as values.
    */
    void init_operations();

    /**
    * @brief Processes the next token and updates the expression tree stack accordingly.
    *
    * This method is called repeatedly by parse() to consume tokens until the entire expression is parsed.
    *
    * @param stack A reference to the stack that holds partial expression trees.
    */
    void parse_next_token(std::stack<std::unique_ptr<Node>>& stack);

private:

    Lexer& m_lexer;
    std::unordered_map<std::string, rpn::BinaryOperator> m_operations;
};

} // namespace rpn

#endif // RPN_PARSER_HPP