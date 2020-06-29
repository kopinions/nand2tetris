#ifndef AST_HPP
#define AST_HPP
#include <string>

class node {
public:
  virtual ~node() = default;
};

class anode : public node {
public:
  anode() : _address(-1), _symbol(""){};
  anode(int address) : _address(address), _symbol(""){};
  anode(std::string symbol) : _address(-1), _symbol(symbol){};
  void relocate(int address) { this->_address = address; }
  int address() { return _address; }

private:
  int _address;
  std::string _symbol;
};

class cnode : public node {
public:
  cnode(){};
};

#endif // AST_HPP
