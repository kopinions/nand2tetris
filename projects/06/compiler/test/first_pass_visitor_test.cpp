#include "ast.hpp"
#include "first_pass_visitor.hpp"
#include "parser.hpp"
#include "tokenizer.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(first_pass_visitor_test, should_able_to_generate_a_construction) {
  tokenizer to;
  std::list<token> tokens = to.tokenize("@AA");
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);
  auto ctx = std::make_shared<context>();
  auto v = std::make_shared<first_pass_visitor>(ctx);
  nodes.front()->accept(v);
ASSERT_THAT(*(ctx->defined("AA")), testing::Eq(-1));
}

TEST(first_pass_visitor_test, should_able_to_query_when_label) {
  tokenizer to;
  std::list<token> tokens = to.tokenize("@AA\n"
                                        "(AA)");
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);
  auto ctx = std::make_shared<context>();
  auto v = std::make_shared<first_pass_visitor>(ctx);
  for (auto it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->accept(v);
  }

  ASSERT_THAT(*(ctx->defined("AA")), testing::Eq(1));
}
