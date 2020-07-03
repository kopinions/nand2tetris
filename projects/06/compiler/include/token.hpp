#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

class token {
public:
  enum type { at, number, symbol, jmp, assign, plus, hyphen, ampersand, vbar, exclamation, semicolon, lparen, rparen };

  token(enum type type) : _type(type), _value("") {}
  token(enum type type, std::string value) : _type(type), _value(value) {}

  enum type type() { return _type; }
  std::string value() { return _value; }

  bool is_operator() {
    switch (_type) {
    case token::type::plus:
    case token::type::hyphen:
    case token::type::ampersand:
    case token::type::vbar:
    case token::type::exclamation:
      return true;
    default:
      return false;
    }
  }

private:
  enum type _type;
  std::string _value;
};
#endif // TOKEN_HPP
