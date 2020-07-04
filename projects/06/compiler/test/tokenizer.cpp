#include "tokenizer.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(tokenizer, should_able_to_get_at_token) {
  tokenizer to;
  std::list<token> toks = to.tokenize("@");
  ASSERT_THAT(toks.size(), 1);
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::at));
}

TEST(tokenizer, should_able_to_get_number) {
  tokenizer to;
  std::list<token> toks = to.tokenize("123");
  ASSERT_THAT(toks.size(), 1);
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::number));
  ASSERT_THAT(toks.front().value(), testing::Eq("123"));
}

TEST(tokenizer, should_able_to_get_symbol) {
  tokenizer to;
  std::list<token> toks = to.tokenize("SYMBOL");
  ASSERT_THAT(toks.size(), 1);
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::symbol));
  ASSERT_THAT(toks.front().value(), testing::Eq("SYMBOL"));
}

TEST(tokenizer, should_able_to_get_symbol_with_underscore) {
  tokenizer to;
  std::list<token> toks = to.tokenize("SYMBOL_A");
  ASSERT_THAT(toks.size(), 1);
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::symbol));
  ASSERT_THAT(toks.front().value(), testing::Eq("SYMBOL_A"));
}

TEST(tokenizer, should_able_to_get_symbol_with_dot_and_dollar) {
    tokenizer to;
    std::list<token> toks = to.tokenize("ball.setdestination$if_true0");
    ASSERT_THAT(toks.size(), 1);
    ASSERT_THAT(toks.front().type(), testing::Eq(token::type::symbol));
    ASSERT_THAT(toks.front().value(), testing::Eq("ball.setdestination$if_true0"));
}

TEST(tokenizer, should_able_to_get_the_label) {
  tokenizer to;
  std::list<token> toks = to.tokenize("(SYMBOL)");
  ASSERT_THAT(toks.size(), 3);
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::lparen));
  toks.pop_front();

  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::symbol));
  ASSERT_THAT(toks.front().value(), testing::Eq("SYMBOL"));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::rparen));
}

TEST(tokenizer, should_able_to_parse_a_instruction) {
  tokenizer to;
  std::list<token> toks = to.tokenize("@SYMBOL");
  ASSERT_THAT(toks.size(), 2);
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::at));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::symbol));
  ASSERT_THAT(toks.front().value(), testing::Eq("SYMBOL"));

  toks = to.tokenize("@111");
  ASSERT_THAT(toks.size(), 2);
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::at));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::number));
  ASSERT_THAT(toks.front().value(), testing::Eq("111"));
}

TEST(tokenizer, should_able_to_parse_supported_operators) {
  tokenizer to;
  std::list<token> toks = to.tokenize("=;+-!&|");
  ASSERT_THAT(toks.size(), 7);
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

TEST(tokenizer, should_able_to_parse_c_instruction) {
  tokenizer to;
  std::list<token> toks = to.tokenize("MD=D+1;JLE");
  ASSERT_THAT(toks.size(), 7);

  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::symbol));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::assign));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::symbol));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::plus));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::number));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::semicolon));

  toks.pop_front();
  ASSERT_THAT(toks.front().type(), testing::Eq(token::type::symbol));
}

TEST(tokenizer, should_able_to_bypass_comment) {
  tokenizer to;
  std::list<token> toks = to.tokenize("//this is comment");
  ASSERT_THAT(toks.size(), 0);
}

TEST(tokenizer, should_able_to_bypass_white_space) {
  tokenizer to;
  std::list<token> toks = to.tokenize("   //this is comment");
  ASSERT_THAT(toks.size(), 0);
}
