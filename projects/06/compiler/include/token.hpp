#ifndef TOKEN_HPP
#define TOKEN_HPP
class token {
public:
  enum type { at, eof };

  token(enum type type) : _type(type) {}

  enum type type() { return _type; }

private:
  enum type _type;
};
#endif // TOKEN_HPP
