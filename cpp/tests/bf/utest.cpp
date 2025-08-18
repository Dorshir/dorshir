#include "mu_test.h"
#include "memory.hpp"
#include "io.hpp"
#include "compiler.hpp"
#include "cpu.hpp"
#include "vm.hpp"
#include <string>

BEGIN_TEST(memory_initialization)
    bf::Memory memory(16, 4);
    ASSERT_EQUAL(memory.read(), 0);

    memory.write(42);
    ASSERT_EQUAL(memory.read(), 42);
END_TEST

BEGIN_TEST(memory_expansion)
    bf::Memory memory(4, 4);
    memory.write(10);

    memory.move_right();
    memory.write(20);

    memory.move_right();
    memory.write(30);

    memory.move_left();
    ASSERT_EQUAL(memory.read(), 20);

    memory.move_left();
    ASSERT_EQUAL(memory.read(), 10);

    memory.move_left(); // triggers prepend if pointer == 0
    memory.write(99);
    ASSERT_EQUAL(memory.read(), 99);
END_TEST

//----------------------- IO Tests -----------------------
BEGIN_TEST(io_input_output)
    bf::IO io;
    io.set_input_buffer("Brainfuck");


    ASSERT_EQUAL(io.read_buffered(), 'B');
    ASSERT_EQUAL(io.read_buffered(), 'r');
    ASSERT_EQUAL(io.read_buffered(), 'a');


    io.write_buffered('H');
    io.write_buffered('i');
    ASSERT_EQUAL(io.get_output_buffer(), "Hi");

    io.clear_output_buffer();
    ASSERT_EQUAL(io.get_output_buffer(), "");
END_TEST

BEGIN_TEST(compiler_valid_program)
    std::string source = "+++[->++<]";
    std::vector<OpCode> compiled = bf::Compiler::compile(source);
    ASSERT_EQUAL(compiled.size(), 10);
END_TEST

BEGIN_TEST(compiler_invalid_program_unmatched_opening)
    std::string source = "+++[->++<";
    std::vector<OpCode> compiled = bf::Compiler::compile(source);
    ASSERT_EQUAL(compiled.size(), 0); 
END_TEST

BEGIN_TEST(compiler_invalid_program_unmatched_closing)
    std::string source = "+++->++<]";
    std::vector<OpCode> compiled = bf::Compiler::compile(source);
    ASSERT_EQUAL(compiled.size(), 0); 
END_TEST

BEGIN_TEST(vm_run_program)
    bf::VM vm(16, 4);
    std::string source = "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++..";

    vm.load_and_compile(source);
    vm.run();


    ASSERT_EQUAL(vm.get_io().get_output_buffer(), "AA");

END_TEST

BEGIN_TEST(vm_input_output_program)
    bf::VM vm(16, 4);
    vm.get_io().set_input_buffer("X");

    std::string source = ",++.";
    vm.load_and_compile(source);
    vm.run();

    ASSERT_EQUAL(vm.get_io().get_output_buffer(), "Z");
END_TEST

BEGIN_TEST(hello_world)
    bf::VM vm(256, 8);

    std::string source = 
        "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++."
        ">++.<<+++++++++++++++.>.+++.------.--------.>+.>.";

    vm.load_and_compile(source);
    vm.run();

    ASSERT_EQUAL(vm.get_io().get_output_buffer(), "Hello World!\n");
END_TEST


TEST_SUITE(brainfuck vm tests)
    TEST(memory_initialization)
    TEST(memory_expansion)
    TEST(io_input_output)
    TEST(compiler_valid_program)
    TEST(compiler_invalid_program_unmatched_opening)
    TEST(compiler_invalid_program_unmatched_closing)
    TEST(vm_run_program)
    TEST(vm_input_output_program)
    TEST(hello_world)
END_SUITE
