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
  EXPECT_CALL(*reporter, report("000000000000111")).Times(1);
  nodes.front()->accept(v);
}

TEST(visitor, should_able_to_generate_a_instruction_with_) {
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
  EXPECT_CALL(*reporter, report("000000000000010")).Times(1);
  auto v = std::make_shared<hack_visitor>(reporter, ctx);

  for (auto it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->accept(v);
  }
}