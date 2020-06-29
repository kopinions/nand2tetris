#include "tokenizer.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(tokenizer, should_able_to_get_at_token) {
  tokenizer to;
  std::list<token> toks = to.tokenize("@");
  ASSERT_THAT(toks.size(), 2);
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::at));
}

TEST(tokenizer, should_able_to_get_number) {
  tokenizer to;
  std::list<token> toks = to.tokenize("123");
  ASSERT_THAT(toks.size(), 2);
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::number));
  ASSERT_THAT(toks.front().value(), testing::Eq("123"));
}

TEST(tokenizer, should_able_to_get_symbol) {
  tokenizer to;
  std::list<token> toks = to.tokenize("SYMBOL");
  ASSERT_THAT(toks.size(), 2);
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::symbol));
  ASSERT_THAT(toks.front().value(), testing::Eq("SYMBOL"));
}

TEST(tokenizer, should_able_to_parse_a_instruction) {
  tokenizer to;
  std::list<token> toks = to.tokenize("@SYMBOL");
  ASSERT_THAT(toks.size(), 3);
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::at));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::symbol));
  ASSERT_THAT(toks.front().value(), testing::Eq("SYMBOL"));

  toks = to.tokenize("@111");
  ASSERT_THAT(toks.size(), 3);
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::at));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::number));
  ASSERT_THAT(toks.front().value(), testing::Eq("111"));
}

TEST(tokenizer, should_able_to_parse_supported_operators) {
  tokenizer to;
  std::list<token> toks = to.tokenize("=;+-!&|");
  ASSERT_THAT(toks.size(), 8);
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::assign));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::semicolon));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::plus));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::hyphen));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::exclamation));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::ampersand));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::vbar));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
