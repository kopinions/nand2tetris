#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

class token {
public:
  enum type {
    at,
    number,
    symbol,
    jmp,
    assign,
    plus,
    hyphen,
    ampersand,
    vbar,
    exclamation,
    semicolon,
    eof
  };

  token(enum type type) : _type(type), _value("") {}
  token(enum type type, std::string value) : _type(type), _value(value) {}

  enum type type() { return _type; }
  std::string value() { return _value; }

private:
  enum type _type;
  std::string _value;
};
#endif // TOKEN_HPP
