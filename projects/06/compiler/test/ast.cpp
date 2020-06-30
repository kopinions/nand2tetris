#include "ast.hpp"
#include "parser.hpp"
#include "tokenizer.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(tokenizer, should_able_to_parse_a_instruction) {
  tokenizer to;
  std::list<token> tokens = to.tokenize("@111");
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);
  ASSERT_THAT(nodes.size(), testing::Eq(1));
  auto a = dynamic_cast<anode *>(nodes.front().get());
  ASSERT_THAT(a, testing::NotNull());
  ASSERT_THAT(a->address(), testing::Eq(111));
}

TEST(tokenizer, should_able_to_parse_a_instruction_with_symbol) {
  tokenizer to;
  std::list<token> tokens = to.tokenize("@SYMBOL");
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);
  ASSERT_THAT(nodes.size(), testing::Eq(1));
  auto a = dynamic_cast<anode *>(nodes.front().get());
  ASSERT_THAT(a, testing::NotNull());
  ASSERT_THAT(a->address(), testing::Eq(-1));
}

TEST(tokenizer, should_able_to_parse_c_instruction) {
  tokenizer to;
  std::list<token> tokens = to.tokenize("MD=D;JLE");
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);
  ASSERT_THAT(nodes.size(), testing::Eq(1));
  auto a = dynamic_cast<cnode *>(nodes.front().get());
  ASSERT_THAT(a, testing::NotNull());
}

TEST(tokenizer, should_able_to_parse_c_instruction_computation_with_jump) {
  tokenizer to;
  std::list<token> tokens = to.tokenize("M;JLE");
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);
  ASSERT_THAT(nodes.size(), testing::Eq(1));
  auto a = dynamic_cast<cnode *>(nodes.front().get());
  ASSERT_THAT(a, testing::NotNull());
}

TEST(tokenizer, should_able_to_parse_c_instruction_complex_computation_with_jump) {
  tokenizer to;
  std::list<token> tokens = to.tokenize("D-1;JLE");
  parser parser;
  std::list<std::unique_ptr<node>> nodes = parser.parse(tokens);
  ASSERT_THAT(nodes.size(), testing::Eq(1));
  auto a = dynamic_cast<cnode *>(nodes.front().get());
  ASSERT_THAT(a, testing::NotNull());
}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
