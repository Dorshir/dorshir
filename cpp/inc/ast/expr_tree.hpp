#ifndef EXPR_TREE_HPP
#define EXPR_TREE_HPP

#include <iostream>
#include <cstdint>
#include <string>
#include <stack>
#include <unordered_map>
#include <memory>
#include <vector>

namespace ast {

class ExpressionTree {

public:
    
    enum Operator { PLUS, MUL, SUB, DIV, KONST, VAR };

    struct Node {
        Operator op;
        union {
            double value;
            std::string variable;
            struct {
                std::unique_ptr<Node> left;
                std::unique_ptr<Node> right;
            };
        };

        Node(double val);
        Node(Operator op, std::unique_ptr<Node> left, std::unique_ptr<Node> right);
        Node(std::string const& var);
    };

private:

    double compute(std::unordered_map<std::string, double> const& context);
    void print(std::unique_ptr<Node> const& root, bool postfix);

private:

    std::unique_ptr<Node> root;

};

} // namespace ast

#endif // EXPR_TREE_HPP
