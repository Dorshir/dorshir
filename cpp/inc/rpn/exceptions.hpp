#ifndef RPN_EXCEPTIONS_HPP
#define RPN_EXCEPTIONS_HPP

#include <stdexcept>

namespace rpn {

struct UndefinedVariable : std::exception {};

struct MissingOperand : std::exception {};

struct UndefinedVariant : std::exception {};

struct InvalidExpression : std::exception {};

struct ZeroDivision : std::exception {};

struct OutOfRange : std::exception {};

} // namespace rpn

#endif // RPN_EXCEPTIONS_HPP