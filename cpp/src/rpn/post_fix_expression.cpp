#include "post_fix_expression.hpp"
#include "exceptions.hpp"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

namespace rpn {

int evaluate(std::unique_ptr<Node> const& node, std::unordered_map<std::string, int> const& variables)
{
    if (std::holds_alternative<int>(node->value)) {
        return std::get<int>(node->value);
    }
    else if (std::holds_alternative<std::string>(node->value)) {

        auto const& var = std::get<std::string>(node->value);
        auto it = variables.find(var);

        if (it != variables.end()) {
            return it->second;
        }

        throw UndefinedVariable();
    }
    else if (std::holds_alternative<BinaryOperator>(node->value)) {

        if (node->left == nullptr || node->right == nullptr) {
            throw MissingOperand();
        }

        int left_val = evaluate(node->left, variables);
        int right_val = evaluate(node->right, variables);

        return std::get<BinaryOperator>(node->value)(left_val, right_val);
    }

    throw UndefinedVariant();
}


} // namespace rpn
