#ifndef RPN_NODE_HPP
#define RPN_NODE_HPP

#include <functional>
#include <memory>
#include <string>
#include <variant>

namespace rpn {

/**
* @brief A binary operator function type.
*
* Represents a function that takes two integers and returns an integer result.
*/
using BinaryOperator = std::function<int(int, int)>;

/**
* @brief Variant type to store a node's value.
*
* The Value can be one of the following:
* - An integer literal.
* - A variable name (std::string).
* - A binary operator (BinaryOperator).
*/
using Value = std::variant<int, std::string, BinaryOperator>;


/**
* @brief Node structure representing a node in an expression tree.
*
* Each Node stores a value (which can be an integer, a variable name, or a binary operator)
* and pointers to its left and right child nodes.
*/
struct Node {
    
    explicit Node(Value value)
    : value{std::move(value)}
    , left{nullptr}
    , right{nullptr}
    {}

    Node(BinaryOperator op, std::unique_ptr<Node> left, std::unique_ptr<Node> right)
    : value{std::move(op)}
    , left{std::move(left)}
    , right{std::move(right)}
    {}

    ~Node() = default;
    Node(const Node&) = default;
    Node& operator=(const Node&) = default;
    Node(Node&&) noexcept = default;
    Node& operator=(Node&&) noexcept = default;


    Value value;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

} // namespace rpn

#endif // RPN_NODE_HPP