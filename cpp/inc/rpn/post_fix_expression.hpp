#ifndef RPN_POST_FIX_EXPRESSION_HPP
#define RPN_POST_FIX_EXPRESSION_HPP

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

#include "node.hpp"

namespace rpn {

int evaluate(std::unique_ptr<Node> const& node, std::unordered_map<std::string, int> const& variables = {});

// void print_infix(std::unique_ptr<Node> const& node);

} // namespace rpn

#endif // RPN_POST_FIX_EXPRESSION_HPP