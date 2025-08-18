#include "mu_test.h"
#include "lexer.hpp"
#include "parser.hpp"
#include "post_fix_expression.hpp"

#include <unordered_map>
#include <string>


BEGIN_TEST(lexer_basic)
{
    rpn::Lexer lexer{"45 4 +"}; 
    ASSERT_EQUAL(lexer.next(), "45");
    ASSERT_EQUAL(lexer.next(), "4");
    ASSERT_EQUAL(lexer.next(), "+");

    try {
        auto a = lexer.next();
        ASSERT_FAIL(a.empty());
    } catch (...) {
        ASSERT_PASS();
    }
}
END_TEST

BEGIN_TEST(parser_basic_addition)
{
    rpn::Lexer lexer{"45 4 +"};
    rpn::Parser parser{lexer};
    
    auto ast = parser.parse();
    ASSERT_THAT(ast);
}
END_TEST

BEGIN_TEST(eval_basic)
{
    std::string input = "15 2 * 10 5 - *";
    rpn::Lexer lexer{input};
    rpn::Parser parser{lexer};
    
    auto ast = parser.parse();
    ASSERT_THAT(ast);

    ASSERT_EQUAL(rpn::evaluate(ast), 150);
}
END_TEST

BEGIN_TEST(eval_with_var)
{
    std::string input = "331 42 * 19 x - *";
    rpn::Lexer lexer{input};
    rpn::Parser parser{lexer};
    
    auto ast = parser.parse();
    ASSERT_THAT(ast);

    std::unordered_map<std::string, int> variables = { {"x", 5} };
    ASSERT_EQUAL(rpn::evaluate(ast, variables), 194628);
}
END_TEST



TEST_SUITE(Post fix expr unit tests)

    TEST(lexer_basic)
    TEST(parser_basic_addition)
    TEST(eval_basic)
    TEST(eval_with_var)

END_SUITE
