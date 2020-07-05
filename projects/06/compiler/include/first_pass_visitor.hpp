#ifndef FIRST_PASS_VISITOR_HPP
#define FIRST_PASS_VISITOR_HPP
#include "ast.hpp"
#include "context.hpp"
#include "visitor.hpp"
#include <iostream>
#include <memory>

class first_pass_visitor : public visitor {
public:
  first_pass_visitor(std::shared_ptr<context> ctx) : _ctx(ctx), _pc(0){};

  virtual void visit(anode *n) {
    _pc++;
  };

  virtual void visit(cnode *) { _pc++; };

  virtual void visit(constant *){

  };
  virtual void visit(unary *){

  };

  virtual void visit(binary *){

  };

  virtual void visit(label *l) { _ctx->define(l->name(), _pc); };

  virtual void visit(expression *){

  };

  virtual void visit(std::shared_ptr<context>){

  };

private:
  std::shared_ptr<context> _ctx;
  int _pc;
};
#endif // FIRST_PASS_VISITOR_HPP
