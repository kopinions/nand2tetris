#include "ast.hpp"
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
  auto v = std::make_shared<hack_visitor>(reporter);
  EXPECT_CALL(*reporter, report("000000000000111")).Times(1);
  nodes.front()->accept(v);
}
