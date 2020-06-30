#ifndef AST_HPP
#define AST_HPP
#include "visitor.hpp"
#include <string>

class node {
public:
  virtual ~node() = default;
  virtual void accept(visitor visitor) = 0;
};

class anode : public node {
public:
  anode() : _address(-1), _symbol(""){};
  anode(int address) : _address(address), _symbol(""){};
  anode(std::string symbol) : _address(-1), _symbol(symbol){};
  virtual void accept(visitor visitor) { visitor.visit(this); }
  void relocate(int address) { this->_address = address; }
  int address() { return _address; }

private:
  int _address;
  std::string _symbol;
};

class expression: public node {
public:
  virtual void accept(visitor visitor) { visitor.visit(this); }
};

class cnode : public node {
public:
  cnode(expression e): _dest(""), _expression(e), _jmp("") {};
  cnode(expression e, std::string jmp): _dest(""), _expression(e), _jmp(jmp) {};
  cnode(std::string dest, expression e): _dest(dest), _expression(e), _jmp("") {};
  cnode(std::string dest, expression e, std::string jmp): _dest(dest), _expression(e), _jmp(jmp) {};
  virtual void accept(visitor visitor) { visitor.visit(this); }
private:
  std::string _dest;
  expression _expression;
  std::string _jmp;
};

#endif // AST_HPP
