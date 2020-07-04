#include "ast.hpp"
#include "context.hpp"
#include "first_pass_visitor.hpp"
#include "hack_visitor.hpp"
#include "include/mock_reporter.hpp"
#include "parser.hpp"
#include "tokenizer.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(visitor, should_able_to_generate_a_construction) {
  tokenizer to;
  std::list<token> tokens = to.tokenize("@7");
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);
  auto reporter = std::make_shared<mock_reporter>();
  auto ctx = std::make_shared<context>();
  auto v = std::make_shared<hack_visitor>(reporter, ctx);
  EXPECT_CALL(*reporter, report("0000000000000111")).Times(1);
  nodes.front()->accept(v);
}

TEST(visitor, should_able_to_generate_a_instruction_with_at_label) {
  tokenizer to;
  std::list<token> tokens = to.tokenize("@AA\n"
                                        "D=M\n"
                                        "(AA)\n"
                                        "D=A");
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);

  auto ctx = std::make_shared<context>();
  auto first_pass = std::make_shared<first_pass_visitor>(ctx);

  for (auto it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->accept(first_pass);
  }

  auto reporter = std::make_shared<mock_reporter>();
  EXPECT_CALL(*reporter, report(testing::AnyOf(testing::Eq("0000000000000010"), testing::Eq("1110101110000000"),
                                               testing::Eq("1110100110000000"))))
      .Times(3);
  auto v = std::make_shared<hack_visitor>(reporter, ctx);

  for (auto it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->accept(v);
  }
}

TEST(visitor, should_able_to_generate_a_instruction_with_at_variable) {
  tokenizer to;
  std::list<token> tokens = to.tokenize("@AA\n"
                                        "D=M\n"
                                        "D=A");
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);

  auto ctx = std::make_shared<context>();
  auto first_pass = std::make_shared<first_pass_visitor>(ctx);

  for (auto it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->accept(first_pass);
  }

  auto reporter = std::make_shared<mock_reporter>();
  auto v = std::make_shared<hack_visitor>(reporter, ctx);
  EXPECT_CALL(*reporter, report(testing::AnyOf(testing::Eq("0000000000010001"), testing::Eq("1110101110000000"),
                                               testing::Eq("1110100110000000"))))
      .Times(3);

  for (auto it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->accept(v);
  }
}

TEST(visitor, should_able_to_generate_c_instruction) {
  tokenizer to;
  std::list<token> tokens = to.tokenize("M=D+1;JMP");
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);

  auto ctx = std::make_shared<context>();
  auto first_pass = std::make_shared<first_pass_visitor>(ctx);

  for (auto it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->accept(first_pass);
  }

  auto reporter = std::make_shared<mock_reporter>();
  auto v = std::make_shared<hack_visitor>(reporter, ctx);
  EXPECT_CALL(*reporter, report("1110010011111111")).Times(1);

  for (auto it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->accept(v);
  }
}

TEST(visitor, should_able_to_generate_c_instruction_without_dist) {
  tokenizer to;
  std::list<token> tokens = to.tokenize("D+1;JMP");
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);

  auto ctx = std::make_shared<context>();
  auto first_pass = std::make_shared<first_pass_visitor>(ctx);

  for (auto it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->accept(first_pass);
  }

  auto reporter = std::make_shared<mock_reporter>();
  auto v = std::make_shared<hack_visitor>(reporter, ctx);
  EXPECT_CALL(*reporter, report("1110000011111111")).Times(1);

  for (auto it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->accept(v);
  }
}

TEST(visitor, should_able_to_generate_c_instruction_unconditional_jump) {
  tokenizer to;
  std::list<token> tokens = to.tokenize("0;JMP");
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);

  auto ctx = std::make_shared<context>();
  auto first_pass = std::make_shared<first_pass_visitor>(ctx);

  for (auto it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->accept(first_pass);
  }

  auto reporter = std::make_shared<mock_reporter>();
  auto v = std::make_shared<hack_visitor>(reporter, ctx);
  EXPECT_CALL(*reporter, report("1110000101010111")).Times(1);

  for (auto it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->accept(v);
  }
}