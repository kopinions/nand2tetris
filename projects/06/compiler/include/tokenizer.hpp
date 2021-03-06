#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP
#include "token.hpp"
#include <iostream>
#include <list>
#include <string>

class tokenizer {
public:
  std::list<token> tokenize(std::string code) {
    std::list<token> tokens;

    for (auto iter = code.begin(); iter != code.end(); iter++) {
      if (std::isspace(*iter)) {
        continue;
      }

      if (*iter == '/') {
        if (*(++iter) == '/') {
          iter++;
          while (*iter != '\n' && *iter != '\r' && *iter != '\0') {
            iter++;
          }
          iter--;
        } else {
          std::cerr << "Unrecognized char '" << *iter << "'"
                    << " behind /;";
        }
        continue;
      }
      if (*iter == '@') {
        tokens.push_back(token(token::type::at));
        continue;
      }

      if (std::isdigit(*iter)) {
        std::string number;
        number += *iter;

        while (std::isdigit(*(++iter))) {
          number += *iter;
        }
        tokens.push_back(token(token::type::number, number));
        iter--;
        continue;
      }

      if (std::isalpha(*iter)) {
        std::string symbol;
        symbol += *iter;
        iter++;
        while (std::isalnum(*iter) || *iter == '_' || *iter == '$' || *iter =='.') {
          symbol += *iter;
          iter++;
        }
        tokens.push_back(token(token::type::symbol, symbol));
        iter--;
        continue;
      }

      switch (*iter) {
      case '=':
        tokens.push_back(token(token::type::assign, "="));
        continue;
      case '+':
        tokens.push_back(token(token::type::plus, "+"));
        continue;
      case '-':
        tokens.push_back(token(token::type::hyphen, "-"));
        continue;
      case '!':
        tokens.push_back(token(token::type::exclamation, "!"));
        continue;
      case '|':
        tokens.push_back(token(token::type::vbar, "|"));
        continue;
      case ';':
        tokens.push_back(token(token::type::semicolon, ";"));
        continue;
      case '&':
        tokens.push_back(token(token::type::ampersand, "&"));
        continue;
      case '(':
        tokens.push_back(token(token::type::lparen, "("));
        continue;
      case ')':
        tokens.push_back(token(token::type::rparen, ")"));
        continue;
      default:
        std::cerr << "Unrecognized operator:"
                  << "'" << (int)*iter << "'" << std::endl;
        continue;
      }
    }
    return tokens;
  }
};
#endif // TOKENIZER_HPP
