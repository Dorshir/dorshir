#include "parser.hpp"
#include "lexer.hpp"
#include "exceptions.hpp"

#include <string_view>
#include <vector>
#include <stdexcept>
#include <stack>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <string>


namespace rpn {

Parser::Parser(Lexer& lexer)
: m_lexer{lexer}
{
    init_operations();
}


std::unique_ptr<Node> Parser::parse()
{
    std::stack<std::unique_ptr<Node>> stack;
    while (true) {
        try {
            parse_next_token(stack);
            
        } catch (std::range_error const&) {
            break;
        }
    }

    if (stack.size() != 1) {
        throw InvalidExpression();
    }

    return std::move(stack.top());
    
}


bool Parser::is_number(std::string_view tok) 
{
    return !tok.empty() && std::all_of(tok.begin(), tok.end(), ::isdigit);
}


bool Parser::is_operator(std::string_view tok) 
{
    return !tok.empty() && (tok == "+" || tok == "-" || tok == "*" || tok == "/");
}


void Parser::init_operations()
{
    m_operations.emplace("+", [](int a, int b){return a + b;});
    m_operations.emplace("-", [](int a, int b){return a - b;});
    m_operations.emplace("*", [](int a, int b){return a * b;});
    m_operations.emplace("/", [](int a, int b)
    {
        if (b == 0) {
            throw ZeroDivision();
        }
        return a / b;
    });
}


void Parser::parse_next_token(std::stack<std::unique_ptr<Node>>& stack)
{
    auto tok = m_lexer.next();

    if (is_number(tok)) {
        stack.push(std::make_unique<Node>(Value{std::stoi(std::string(tok))}));
    }
    else if (is_operator(tok)) {
        if (stack.size() < 2) {
            throw InvalidExpression();
        }

        auto right = std::move(stack.top()); 
        stack.pop();
        auto left = std::move(stack.top()); 
        stack.pop();

        stack.push(std::make_unique<Node>(m_operations.at(std::string(tok)), std::move(left), std::move(right)));
    }
    else {
        stack.push(std::make_unique<Node>(Value{std::string(tok)}));
    }

    
}

} // namespace rpn