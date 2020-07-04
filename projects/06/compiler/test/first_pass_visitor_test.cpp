#include "ast.hpp"
#include "first_pass_visitor.hpp"
#include "mock_context.hpp"
#include "parser.hpp"
#include "tokenizer.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(first_pass_visitor_test, should_able_to_generate_a_construction) {
  tokenizer to;
  std::list<token> tokens = to.tokenize("@AA");
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);
  auto ctx = std::make_shared<mock_context>();
  auto v = std::make_shared<first_pass_visitor>(ctx);
  EXPECT_CALL(*ctx, add("AA")).Times(1);
  nodes.front()->accept(v);
}
