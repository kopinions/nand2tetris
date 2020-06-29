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

class cnode : public node {
public:
  cnode(){};
  virtual void accept(visitor visitor) { visitor.visit(this); }
};

#endif // AST_HPP
