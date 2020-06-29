#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP
#include "token.hpp"
#include <list>
#include <string>

class tokenizer {
public:
  std::list<token> tokenize(std::string code) {
    std::list<token> tokens;

    for (auto iter = code.begin(); iter != code.end(); iter++) {
      if (*iter == '@') {
        tokens.push_back(token(token::type::at));
      }
    }
    tokens.push_back(token(token::type::eof));
    return tokens;
  }
};
#endif // TOKENIZER_HPP
