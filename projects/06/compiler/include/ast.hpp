#ifndef AST_HPP
#define AST_HPP
#include "visitor.hpp"
#include <string>

class node {
public:
  virtual ~node() = default;
  virtual void accept(std::shared_ptr<visitor> v) = 0;
};

class anode : public node {
public:
  anode() : _address(-1), _symbol(""){};
  anode(int address) : _address(address), _symbol(""){};
  anode(std::string symbol) : _address(-1), _symbol(symbol){};
  virtual void accept(std::shared_ptr<visitor> v) { v->visit(this); }
  void relocate(int address) { this->_address = address; }
  int address() { return _address; }
  std::string symbol() { return _symbol; }

private:
  int _address;
  std::string _symbol;
};

class expression : public node {
public:
  virtual void accept(std::shared_ptr<visitor> v) = 0;
};

class unary : public expression {
public:
  unary(std::string op, std::string symbol) : _op(op), _symbol(symbol) {}
  virtual void accept(std::shared_ptr<visitor> v) { v->visit(this); }
  std::string exp() { return _op + _symbol; }

private:
  std::string _op;
  std::string _symbol;
};

class binary : public expression {
public:
  binary(std::string operand1, std::string op, std::string symbol) : _operand1(operand1), _op(op), _symbol(symbol) {}
  virtual void accept(std::shared_ptr<visitor> v) { v->visit(this); }
  std::string exp() { return _operand1 + _op + _symbol; }

private:
  std::string _operand1;
  std::string _op;
  std::string _symbol;
};

class label : public node {
public:
  label(std::string name) : _name(name){};
  virtual void accept(std::shared_ptr<visitor> v) { v->visit(this); }
  std::string name() { return _name; }

private:
  std::string _name;
};

class constant : public node {
public:
  constant(int number) : _number(number){};
  virtual void accept(std::shared_ptr<visitor> v) { v->visit(this); }
  int number() { return _number; }

private:
  int _number;
};

class cnode : public node {
public:
  cnode(std::unique_ptr<node> e) : _dest(""), _expression(std::move(e)), _jmp(""){};
  cnode(std::unique_ptr<node> e, std::string jmp) : _dest(""), _expression(std::move(e)), _jmp(jmp){};
  cnode(std::string dest, std::unique_ptr<node> e) : _dest(dest), _expression(std::move(e)), _jmp(""){};
  cnode(std::string dest, std::unique_ptr<node> e, std::string jmp)
      : _dest(dest), _expression(std::move(e)), _jmp(jmp){};
  virtual void accept(std::shared_ptr<visitor> v) { v->visit(this); }

  std::string dest() { return _dest; }
  std::string jmp() { return _jmp; }
  node *e() { return _expression.get(); }

private:
  std::string _dest;
  std::unique_ptr<node> _expression;
  std::string _jmp;
};

#endif // AST_HPP
